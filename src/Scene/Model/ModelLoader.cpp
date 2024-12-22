#include "ModelLoader.h"
#include "Model.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoader.h"
#include "Engine/Node.h"
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
		const aiScene* scene = importer.ReadFile(creationParam->getBlobLoader()->normalizedPath(creationParam), aiProcess_ConvertToLeftHanded
			| aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_ImproveCacheLocality | aiProcess_SortByPType);
		
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			asset->loadFailed__();
			LOG_ERROR("ERROR::ASSIMP::{0}", importer.GetErrorString());
			//m_mtx.unlock();
			return;
		}

		auto model = std::dynamic_pointer_cast<Model>(asset);
		model->m_node = copyTree(nullptr, scene->mRootNode);
		asset->loadSucceeded__();
		//m_mtx.unlock();
	}

	std::shared_ptr<Node> ModelLoader::copyTree(std::shared_ptr<Node> myNodeParent, aiNode* otherNode)
	{
		if (!otherNode)
		{
			return nullptr;
		}

		std::shared_ptr<Node> myNode = std::make_shared<Node>();
		myNode->set_name(otherNode->mName.C_Str());
		myNode->addToParent(myNodeParent);

		Transform transform;
		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		memcpy_s(&worldMatrix, sizeof(DirectX::XMMATRIX), &otherNode->mTransformation, sizeof(aiMatrix4x4));
		transform.setWorldMatrix(worldMatrix);
		myNode->set_transform(transform);

		for (unsigned int i = 0; i < otherNode->mNumChildren; i++)
		{
			copyTree(myNode, otherNode->mChildren[i]);
		}

		return myNode;
	}
}