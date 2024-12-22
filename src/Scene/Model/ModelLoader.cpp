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
		//m_mtx.lock();

		if (!asset || !std::dynamic_pointer_cast<Model>(asset))
		{
			asset->loadFailed__();
			//m_mtx.unlock();
			return;
		}

		if (asset->isLoadingSucceed())
		{
			//m_mtx.unlock();
			return;
		}

		auto creationParam = std::dynamic_pointer_cast<BlobHolder>(asset->getCreationParam());
		if (!creationParam || ! creationParam->getBlobLoader())
		{
			asset->loadFailed__();
			//m_mtx.unlock();
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
			//m_mtx.unlock();
			return;
		}

		auto model = std::dynamic_pointer_cast<Model>(asset);
		model->m_node = copyTree(aiScene, nullptr, aiScene->mRootNode);
		asset->loadSucceeded__();
		//m_mtx.unlock();
	}

	std::shared_ptr<Node> ModelLoader::copyTree(const aiScene* otherScene, std::shared_ptr<Node> myNodeParent, aiNode* otherNode)
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
		}

		Transform transform;
		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		memcpy_s(&worldMatrix, sizeof(DirectX::XMMATRIX), &otherNode->mTransformation, sizeof(aiMatrix4x4));
		transform.setWorldMatrix(worldMatrix);
		myNode->set_transform(transform);

		for (unsigned int i = 0; i < otherNode->mNumChildren; i++)
		{
			copyTree(otherScene, myNode, otherNode->mChildren[i]);
		}

		return myNode;
	}

	std::shared_ptr<RenderPass> ModelLoader::getRenderPass()
	{
		auto renderer = std::make_shared<Renderer>("builtin://renderer/model.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->load(0);

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

		std::vector<DirectX::XMFLOAT3> positions;
		std::vector<PositionNormalTexcoord> vertices;
		vertices.resize(otherMesh->mNumVertices);
		if (otherMesh->HasPositions() && otherMesh->HasNormals() && otherMesh->HasTextureCoords(0))
		{
			for (unsigned int i = 0; i < otherMesh->mNumVertices; i++)
			{
				vertices[i].position.x = otherMesh->mVertices[i].x;
				vertices[i].position.y = otherMesh->mVertices[i].y;
				vertices[i].position.z = otherMesh->mVertices[i].z;
				positions.push_back(vertices[i].position);

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

		DirectX::BoundingBox aabb;
		DirectX::BoundingBox::CreateFromPoints(aabb, positions.size(), positions.data(), 0);

		Mesh::DrawCall drawCall;
		drawCall.drawMethod = Mesh::DrawMethod::DrawIndexed;
		drawCall.primitiveTopology = Mesh::PrimitiveTopology::TriangleList;
		drawCall.indexCount = (unsigned int)indices.size();

		std::shared_ptr<Mesh> myMesh = std::make_shared<Mesh>(aabb, drawCall, vertexBuffer, indexBuffer);
		myMesh->load(0);
		return myMesh;
	}
}