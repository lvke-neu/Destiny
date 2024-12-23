#include "ModelLoader.h"
#include "Model.h"
#include "Material.h"
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
#include <queue>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
		m_mtx.lock();

		if (!asset || !std::dynamic_pointer_cast<Model>(asset))
		{
			asset->loadFailed__();
			m_mtx.unlock();
			return;
		}

		if (asset->isLoadingSucceed())
		{
			m_mtx.unlock();
			return;
		}

		auto creationParam = std::dynamic_pointer_cast<BlobHolder>(asset->getCreationParam());
		if (!creationParam || ! creationParam->getBlobLoader())
		{
			asset->loadFailed__();
			m_mtx.unlock();
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
			m_mtx.unlock();
			return;
		}

		DirectX::BoundingBox mergedAABB{ { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
		auto model = std::dynamic_pointer_cast<Model>(asset);
		model->m_node = copyTree(aiScene, nullptr, aiScene->mRootNode, mergedAABB);
		std::queue<std::shared_ptr<Node>> nodes;
		nodes.push(model->m_node);
		while (!nodes.empty())
		{
			auto topNode = nodes.front();
			nodes.pop();
			if (topNode)
			{
				for (const auto& component : topNode->getComponents())
				{
					auto visualComponent = std::dynamic_pointer_cast<VisualComponent>(component);
					if (visualComponent && visualComponent->getVisual() && visualComponent->getVisual()->getMesh())
					{
						visualComponent->getVisual()->getMesh()->setBoundingBox(mergedAABB);
					}
				}
				for (const auto& node : topNode->getChilds())
				{
					nodes.push(node);
				}
			}
		}
		asset->loadSucceeded__();
		m_mtx.unlock();
	}

	std::shared_ptr<Node> ModelLoader::copyTree(const aiScene* otherScene, std::shared_ptr<Node> myNodeParent, aiNode* otherNode, DirectX::BoundingBox& mergedAABB)
	{
		if (!otherScene || !otherNode)
		{
			return nullptr;
		}

		std::shared_ptr<Node> myNode = std::make_shared<Node>();
		myNode->set_name(otherNode->mName.C_Str());
		myNode->addToParent(myNodeParent);

		for (unsigned int i = 0; i < otherNode->mNumMeshes; i++)
		{
			std::shared_ptr<VisualComponent> visualComponent = std::make_shared<VisualComponent>();
			visualComponent->setRenderPass(getRenderPass());
			visualComponent->setMesh(getMesh(otherScene->mMeshes[otherNode->mMeshes[i]]));
			myNode->addComponent(visualComponent);
			
			auto material = getMaterial(otherScene->mMaterials[otherScene->mMeshes[otherNode->mMeshes[i]]->mMaterialIndex]);
			if (material)
			{
				auto s_sampler = std::make_shared<SamplerState>();
				s_sampler->load(0);
				visualComponent->getVisual()->getRenderPass()->getRenderer()->setConstant("c_has_c_ambient", material->c_has_c_ambient);
				visualComponent->getVisual()->getRenderPass()->getRenderer()->setConstant("c_has_c_diffuse", material->c_has_c_diffuse);
				visualComponent->getVisual()->getRenderPass()->getRenderer()->setConstant("c_has_t_ambient", material->c_has_t_ambient);
				visualComponent->getVisual()->getRenderPass()->getRenderer()->setConstant("c_has_t_diffuse", material->c_has_t_diffuse);
				visualComponent->getVisual()->getRenderPass()->getRenderer()->setConstant("c_ambient", material->c_ambient);
				visualComponent->getVisual()->getRenderPass()->getRenderer()->setConstant("c_diffuse", material->c_diffuse);
				visualComponent->getVisual()->getRenderPass()->getRenderer()->setSamplerSate("s_sampler", s_sampler);
				visualComponent->getVisual()->getRenderPass()->getRenderer()->setShaderResource("t_ambient", material->t_ambient);
				visualComponent->getVisual()->getRenderPass()->getRenderer()->setShaderResource("t_diffuse", material->t_diffuse);
			}
		

			DirectX::BoundingBox::CreateMerged(mergedAABB, mergedAABB, visualComponent->getVisual()->getMesh()->getBoundingBox());
		}

		Transform transform;
		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		memcpy_s(&worldMatrix, sizeof(DirectX::XMMATRIX), &otherNode->mTransformation, sizeof(aiMatrix4x4));
		transform.setWorldMatrix(worldMatrix);
		myNode->set_transform(transform);

		for (unsigned int i = 0; i < otherNode->mNumChildren; i++)
		{
			copyTree(otherScene, myNode, otherNode->mChildren[i], mergedAABB);
		}

		return myNode;
	}

	std::shared_ptr<RenderPass> ModelLoader::getRenderPass()
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/model.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load();

		std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->setRendererCategory(RenderPass::ForwardOpaque);
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		return renderPass;
	}

	std::shared_ptr<Mesh> ModelLoader::getMesh(aiMesh* otherMesh)
	{
		if (!otherMesh)
		{
			return nullptr;
		}

		std::shared_ptr<Blob> data = nullptr;

		std::vector<PositionNormalTexcoord> vertices;
		vertices.resize(otherMesh->mNumVertices);
		if (otherMesh->HasPositions() && otherMesh->HasNormals() && otherMesh->HasTextureCoords(0))
		{
			for (unsigned int i = 0; i < otherMesh->mNumVertices; i++)
			{
				vertices[i].position.x = otherMesh->mVertices[i].x;
				vertices[i].position.y = otherMesh->mVertices[i].y;
				vertices[i].position.z = otherMesh->mVertices[i].z;

				vertices[i].normal.x = otherMesh->mNormals[i].x;
				vertices[i].normal.y = otherMesh->mNormals[i].y;
				vertices[i].normal.z = otherMesh->mNormals[i].z;

				vertices[i].texcoord.x = otherMesh->mTextureCoords[0][i].x;
				vertices[i].texcoord.y = otherMesh->mTextureCoords[0][i].y;
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

		std::shared_ptr<Mesh> myMesh = std::make_shared<Mesh>(drawCall, vertexBuffer, indexBuffer);
		myMesh->load();
		return myMesh;
	}

	std::shared_ptr<Material> ModelLoader::getMaterial(aiMaterial* otherMaterial)
	{
		if (!otherMaterial)
		{
			return nullptr;
		}

		std::shared_ptr<Material>  material = std::make_shared<Material>();
		
		aiColor4D otherColor;
		if (otherMaterial->Get(AI_MATKEY_COLOR_AMBIENT, otherColor) == aiReturn_SUCCESS)
		{
			material->c_has_c_ambient = true;
			material->c_ambient = { otherColor.r, otherColor.g, otherColor.b, otherColor.a };
		}
		if (otherMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, otherColor) == aiReturn_SUCCESS)
		{
			material->c_has_c_diffuse = true;
			material->c_diffuse = { otherColor.r, otherColor.g, otherColor.b, otherColor.a };
		}
	
		aiString otherStr;
		if (otherMaterial->GetTexture(aiTextureType_AMBIENT, 0, &otherStr) == aiReturn_SUCCESS)
		{
			material->c_has_t_ambient = true;
			material->t_ambient = Texture::Create(otherStr.C_Str());
			if (material->t_ambient)
			{
				material->t_ambient->load();
			}
		}
		if (otherMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &otherStr) == aiReturn_SUCCESS)
		{
			material->c_has_t_diffuse = true;
			material->t_diffuse = Texture::Create(otherStr.C_Str());
			if (material->t_diffuse)
			{
				material->t_diffuse->load();
			}
		}
		return material;
	}
}