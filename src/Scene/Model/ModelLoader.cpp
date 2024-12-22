#include "ModelLoader.h"
#include "Model.h"
#include "Engine/BlobHolder.h"

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

		//if (!asset || !std::dynamic_pointer_cast<Model>(asset))
		//{
		//	asset->loadFailed__();
		//	m_mtx.unlock();
		//	return;
		//}

		//if (asset->isLoadingSucceed())
		//{
		//	m_mtx.unlock();
		//	return;
		//}

		//auto creationParam = std::dynamic_pointer_cast<BlobHolder>(asset->getCreationParam());
		//if (!creationParam)
		//{
		//	asset->loadFailed__();
		//	m_mtx.unlock();
		//	return;
		//}

		//auto path = creationParam->getFullPath();

	}
}