#include "Asset.h"
#include "Engine.h"
#include "ThreadPool.h"
#include "AssetLoader.h"
#include "BlobHolder.h"

namespace Destiny
{
	Asset::Asset() : 
		m_assetLoader(nullptr),
		m_blobHolder(nullptr),
		m_state(loading_state_pending)
	{

	}

	void Asset::initialize(std::shared_ptr<AssetLoader> assetLoader, std::shared_ptr<BlobHolder> blobHolder)
	{
		m_assetLoader = assetLoader;
		m_blobHolder = blobHolder;
	}

	void Asset::load(int priority)
	{
		if (m_assetLoader)
		{
			if (priority)
			{
				Engine::GetInstance()->getThreadPool()->commitTask(std::bind(&AssetLoader::loadAsset, m_assetLoader, shared_from_this()));
			}
			else
			{
				m_assetLoader->loadAsset(shared_from_this());
			}
		}
		else
		{
			loadFailed__();
		}
	}

	void Asset::loadSucceeded__()
	{
		m_state = loading_state_succeeded;
	}

	void Asset::loadFailed__()
	{
		m_state = loading_state_failed;
	}
}