#include "ModelLoader.h"
#include "Model.h"
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
		std::vector<PositionNormalTexcoord> vertices;
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
		data.reset(new Blob(vertices.size() * sizeof(PositionNormalTexcoord)));
		data->copyfrom(vertices.data(), vertices.size() * sizeof(PositionNormalTexcoord));
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(InputLayout::Create_PositionNormalTexcoord(), (unsigned int)sizeof(PositionNormalTexcoord), 0, data);
	
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
}