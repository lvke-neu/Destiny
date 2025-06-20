#include "ModelLoader.h"
#include "Model.h"
#include "Animation.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoader.h"
#include "Engine/Node.h"
#include "Graphics/VisualComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include "Graphics/RenderPass.h"
#include "Graphics/VertexDefine.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/InputLayout.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Mesh.h"
#include "Graphics/SamplerState.h"
#include "Graphics/Texture.h"
#include "Graphics/PbrMaterial.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <queue>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <d3d11.h>

namespace Destiny
{
	ModelLoader::ModelLoader()
	{

	}

	ModelLoader::~ModelLoader()
	{

	}

	
	DirectX::XMMATRIX AssimpMatToDXMat(const aiMatrix4x4& aiMat4)
	{
		return 
		{
			aiMat4.a1, aiMat4.a2, aiMat4.a3, aiMat4.a4,
			aiMat4.b1, aiMat4.b2, aiMat4.b3, aiMat4.b4,
			aiMat4.c1, aiMat4.c2, aiMat4.c3, aiMat4.c4,
			aiMat4.d1, aiMat4.d2, aiMat4.d3, aiMat4.d4,
		};
	}

	void ModelLoader::loadAsset(std::shared_ptr<Asset> asset)
	{
		if (!asset || !std::dynamic_pointer_cast<Model>(asset))
		{
			asset->loadFailed__();
			return;
		}

		if (asset->isLoadingSucceed())
		{
			return;
		}

		auto creationParam = std::dynamic_pointer_cast<BlobHolder>(asset->getCreationParam());
		if (!creationParam || ! creationParam->getBlobLoader())
		{
			asset->loadFailed__();
			return;
		}

		Assimp::Importer importer;
		importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
		const aiScene* aiScene = importer.ReadFile(creationParam->getBlobLoader()->normalizedPath(creationParam), aiProcess_ConvertToLeftHanded
			| aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_ImproveCacheLocality | aiProcess_SortByPType);
		
		if (!aiScene || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode)
		{
			asset->loadFailed__();
			LOG_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
			return;
		}

		auto model = std::dynamic_pointer_cast<Model>(asset);
		model->m_node = copyTree(aiScene, nullptr, aiScene->mRootNode, model, creationParam->getPath());


		//animation
		model->m_animator->m_node = model->m_node;
		processAnimation(aiScene, model);
		if (model->m_animator->m_animations.size())
		{
			model->m_animator->set_animationIndex(0);
		}
		
		asset->loadSucceeded__();
	}

	std::shared_ptr<Node> ModelLoader::copyTree(const aiScene* otherScene, std::shared_ptr<Node> myNodeParent, aiNode* otherNode, std::shared_ptr<Model> model, const std::string& path)
	{
		if (!otherScene || !otherNode || !model)
		{
			return nullptr;
		}

		std::shared_ptr<Node> myNode = std::make_shared<Node>();
		myNode->set_name(otherNode->mName.C_Str());
		myNode->addToParent(myNodeParent);
		myNode->set_serializable(false);

		for (unsigned int i = 0; i < otherNode->mNumMeshes; i++)
		{
			std::shared_ptr<VisualComponent> visualComponent = std::make_shared<VisualComponent>();

			visualComponent->setRenderPass(getRenderPass());
			visualComponent->setMesh(getMesh(otherScene->mMeshes[otherNode->mMeshes[i]], model));
			visualComponent->set_material(getMaterial(otherScene->mMaterials[otherScene->mMeshes[otherNode->mMeshes[i]]->mMaterialIndex], path));
			visualComponent->setShadowMesh(visualComponent->getMesh());
			visualComponent->setShadowRenderPass(getRenderPass());
			myNode->addComponent(visualComponent);
			
			model->m_visualComponents.insert(visualComponent);
		}

		Transform transform;
		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		memcpy_s(&worldMatrix, sizeof(DirectX::XMMATRIX), &otherNode->mTransformation, sizeof(aiMatrix4x4));
		transform.setWorldMatrix(worldMatrix);
		myNode->set_transform(transform);

		for (unsigned int i = 0; i < otherNode->mNumChildren; i++)
		{
			copyTree(otherScene, myNode, otherNode->mChildren[i], model, path);
		}

		return myNode;
	}

	std::shared_ptr<RenderPass> ModelLoader::getRenderPass()
	{
		//auto renderer = Renderer::Create("builtin://renderer/forward_pbr.hlsl");
		////auto renderer = Renderer::Create("builtin://renderer/deferred_pbr.hlsl");
		//std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		//std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		//renderPass->setRendererCategory(RendererCategory::ForwardOpaque);
		////renderPass->setRendererCategory(RendererCategory::DeferredOpaque);
		//renderPass->setRenderer(renderer);
		//renderPass->setRenderStates(renderStates);

		return std::make_shared<RenderPass>();
	}

	std::shared_ptr<Mesh> ModelLoader::getMesh(aiMesh* otherMesh, std::shared_ptr<Model> model)
	{
		if (!otherMesh || !model)
		{
			return nullptr;
		}

		std::shared_ptr<Blob> data = nullptr;
		std::vector<DirectX::XMFLOAT3> positions;
		std::vector<PositionNormalTexcoordBone> vertices;
		vertices.resize(otherMesh->mNumVertices);
		if (otherMesh->HasPositions())
		{
			for (unsigned int i = 0; i < otherMesh->mNumVertices; i++)
			{
				vertices[i].position.x = otherMesh->mVertices[i].x;
				vertices[i].position.y = otherMesh->mVertices[i].y;
				vertices[i].position.z = otherMesh->mVertices[i].z;
				positions.push_back(vertices[i].position);
				
				if (otherMesh->HasNormals())
				{
					vertices[i].normal.x = otherMesh->mNormals[i].x;
					vertices[i].normal.y = otherMesh->mNormals[i].y;
					vertices[i].normal.z = otherMesh->mNormals[i].z;
				}

				if (otherMesh->HasTextureCoords(0))
				{
					vertices[i].texcoord.x = otherMesh->mTextureCoords[0][i].x;
					vertices[i].texcoord.y = otherMesh->mTextureCoords[0][i].y;
				}
				else
				{
					vertices[i].texcoord.x = vertices[i].position.x;
					vertices[i].texcoord.y = vertices[i].position.z;
				}
			}
		}

		auto animation = model->m_animator;
		auto skeleton = animation->m_skeleton;
		for (unsigned int i = 0; i < otherMesh->mNumBones; i++) {
			int BoneId = skeleton->GetBoneId(otherMesh->mBones[i]->mName.C_Str());

			if (BoneId == skeleton->boneInfo.size()) {
				BoneInfo bi(AssimpMatToDXMat(otherMesh->mBones[i]->mOffsetMatrix));
				skeleton->boneInfo.push_back(bi);
			}

			for (unsigned j = 0; j < otherMesh->mBones[i]->mNumWeights; j++) {
		
				if (vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds.x == 0 && vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights.x == 0)
				{
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds.x = BoneId;
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights.x = otherMesh->mBones[i]->mWeights[j].mWeight;
					continue;
				}
				if (vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds.y == 0 && vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights.y == 0)
				{
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds.y = BoneId;
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights.y = otherMesh->mBones[i]->mWeights[j].mWeight;
					continue;
				}
				if (vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds.z == 0 && vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights.z == 0)
				{
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds.z = BoneId;
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights.z = otherMesh->mBones[i]->mWeights[j].mWeight;
					continue;
				}
				if (vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds.w == 0 && vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights.w == 0)
				{
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds.w = BoneId;
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights.w = otherMesh->mBones[i]->mWeights[j].mWeight;
					continue;
				}

				if (vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds2.x == 0 && vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights2.x == 0)
				{
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds2.x = BoneId;
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights2.x = otherMesh->mBones[i]->mWeights[j].mWeight;
					continue;
				}
				if (vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds2.y == 0 && vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights2.y == 0)
				{
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds2.y = BoneId;
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights2.y = otherMesh->mBones[i]->mWeights[j].mWeight;
					continue;
				}
				if (vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds2.z == 0 && vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights2.z == 0)
				{
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds2.z = BoneId;
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights2.z = otherMesh->mBones[i]->mWeights[j].mWeight;
					continue;
				}
				if (vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds2.w == 0 && vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights2.w == 0)
				{
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].boneIds2.w = BoneId;
					vertices[otherMesh->mBones[i]->mWeights[j].mVertexId].weights2.w = otherMesh->mBones[i]->mWeights[j].mWeight;
					continue;
				}
			}
		}

		data.reset(new Blob(vertices.size() * sizeof(PositionNormalTexcoordBone)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionNormalTexcoordBone));
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionNormalTexcoordBone(), (unsigned int)sizeof(PositionNormalTexcoordBone), 0, data);
	
		std::vector<unsigned int> indices;
		if (otherMesh->HasFaces())
		{
			for (unsigned int i = 0; i < otherMesh->mNumFaces; i++)
			{
				for (unsigned int j = 0; j < otherMesh->mFaces[i].mNumIndices; j++)
				{
					indices.push_back(otherMesh->mFaces[i].mIndices[j]);
				}
			}
		}
		data.reset(new Blob(indices.size() * sizeof(unsigned int)));
		data->copyfrom(indices.data(), indices.size() * sizeof(unsigned int));
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index32, data);

		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
		drawCall.indexCount = (unsigned int)indices.size();

		DirectX::BoundingBox aabb;
		DirectX::BoundingBox::CreateFromPoints(aabb, positions.size(), positions.data(), 0);
		DirectX::BoundingBox::CreateMerged(model->m_mergedAABB, model->m_mergedAABB, aabb);

		std::shared_ptr<Mesh> myMesh = std::make_shared<Mesh>(drawCall, vertexBuffer, indexBuffer);
		return myMesh;
	}

	std::string reassembleStr(const aiString& otherStr, const std::string& path)
	{
		std::string stdString = otherStr.C_Str();
		if (stdString.find("builtin://") == std::string::npos)
		{
			std::experimental::filesystem::path fullPath(path);
			auto pos = path.find(fullPath.filename().string());
			stdString = path.substr(0, pos) + stdString;
		}

		return stdString;
	}

	std::shared_ptr<Material> ModelLoader::getMaterial(aiMaterial* otherMaterial, const std::string& path)
	{
		if (!otherMaterial)
		{
			return nullptr;
		}

		auto pbrMaterial = PbrMaterial::Create_Default();
		aiString otherStr;
		
		if (otherMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &otherStr) == aiReturn_SUCCESS)
		{
			pbrMaterial->set_albedo(reassembleStr(otherStr, path));
		}
		else
		{
			aiColor4D otherColor;
			if (otherMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, otherColor) == aiReturn_SUCCESS)
			{
				Pixel pixel{ (unsigned char)(otherColor.r * 255), (unsigned char)(otherColor.g * 255), (unsigned char)(otherColor.b * 255), (unsigned char)(otherColor.a * 255) };
				
				auto blob = std::make_shared<Blob>(sizeof(Pixel));
				blob->copyfrom(&pixel, sizeof(Pixel));

				auto texture = Texture::Create2D(DXGI_FORMAT_R8G8B8A8_UNORM, 1, 1, blob, sizeof(Pixel), sizeof(Pixel));
				pbrMaterial->setAlbedo(texture);		
			}
		}

		if (otherMaterial->GetTexture(aiTextureType_NORMALS, 0, &otherStr) == aiReturn_SUCCESS)
		{
			pbrMaterial->set_normal(reassembleStr(otherStr, path));
		}

		if (otherMaterial->GetTexture(aiTextureType_METALNESS, 0, &otherStr) == aiReturn_SUCCESS)
		{
			pbrMaterial->set_metallic(reassembleStr(otherStr, path));
		}

		if (otherMaterial->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &otherStr) == aiReturn_SUCCESS)
		{
			pbrMaterial->set_roughness(reassembleStr(otherStr, path));
		}		
		
		if (otherMaterial->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &otherStr) == aiReturn_SUCCESS)
		{
			pbrMaterial->set_ao(reassembleStr(otherStr, path));
		}

		return pbrMaterial;
	}

	void ModelLoader::processAnimation(const aiScene* otherScene, std::shared_ptr<Model> model)
	{
		model->m_animator->globalInverseTransform = DirectX::XMMatrixInverse(nullptr, AssimpMatToDXMat(otherScene->mRootNode->mTransformation));
		model->m_animator->startTimeMillis = GetTickCount64();

		for (unsigned int i = 0; i < otherScene->mNumAnimations; i++)
		{
			
			std::shared_ptr<Animation> animation = std::make_shared<Animation>();
			animation->name = otherScene->mAnimations[i]->mName.C_Str();
			animation->duration = otherScene->mAnimations[i]->mDuration;
			animation->ticksPerSecond = otherScene->mAnimations[i]->mTicksPerSecond > 0 ? otherScene->mAnimations[i]->mTicksPerSecond : 25.0;

			for (unsigned int j = 0; j < otherScene->mAnimations[i]->mNumChannels; j++)
			{
				const aiNodeAnim* aiChannel = otherScene->mAnimations[i]->mChannels[j];
				std::shared_ptr<Animation::Channel> channel = std::make_shared<Animation::Channel>();
				channel->nodeName = aiChannel->mNodeName.C_Str();

				for (unsigned int k = 0; k < aiChannel->mNumPositionKeys; k++)
				{
					channel->positionKeys.push_back({ aiChannel->mPositionKeys[k].mValue.x, aiChannel->mPositionKeys[k].mValue.y, aiChannel->mPositionKeys[k].mValue.z });
					channel->positionTimes.push_back((float)aiChannel->mPositionKeys[k].mTime);
				}

				for (unsigned int k = 0; k < aiChannel->mNumRotationKeys; k++)
				{
					channel->rotationKeys.push_back({ aiChannel->mRotationKeys[k].mValue.x, aiChannel->mRotationKeys[k].mValue.y,  aiChannel->mRotationKeys[k].mValue.z, aiChannel->mRotationKeys[k].mValue.w });
					channel->rotationTimes.push_back((float)aiChannel->mRotationKeys[k].mTime);
				}

				for (unsigned int k = 0; k < aiChannel->mNumScalingKeys; k++)
				{
					channel->scalingKeys.push_back({ aiChannel->mScalingKeys[k].mValue.x, aiChannel->mScalingKeys[k].mValue.y, aiChannel->mScalingKeys[k].mValue.z });
					channel->scalingTimes.push_back((float)aiChannel->mScalingKeys[k].mTime);
				}

				animation->channels.push_back(channel);
			}

			model->m_animator->m_animations.push_back(animation);
		}
	}
}