#include "Asset.h"
#include "Engine.h"
#include "ThreadPool.h"
#include "AssetLoader.h"

namespace Destiny
{
	Asset::Asset() : 
		m_assetLoader(nullptr),
		m_state(LoadingState::loading_state_pending)
	{

	}

	void Asset::initialize(std::shared_ptr<AssetLoader> assetLoader, std::shared_ptr<Object> creationParam)
	{
		m_assetLoader = assetLoader;
		m_creationParam = creationParam;
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
			if (priority)
			{
				Engine::GetInstance()->getThreadPool()->commitTask(std::bind(&Asset::doLoad, shared_from_this()));
			}
			else
			{
				doLoad();
			}
		}
	}

	void Asset::reload(int priority)
	{
		if (m_assetLoader)
		{
			if (priority)
			{
				Engine::GetInstance()->getThreadPool()->commitTask(std::bind(&AssetLoader::reloadAsset, m_assetLoader, shared_from_this()));
			}
			else
			{
				m_assetLoader->reloadAsset(shared_from_this());
			}
		}
		else
		{
			if (priority)
			{
				Engine::GetInstance()->getThreadPool()->commitTask(std::bind(&Asset::doReload, shared_from_this()));
			}
			else
			{
				reload();
			}
		}
	}

	void Asset::doLoad()
	{
		loadFailed__();
	}

	void Asset::doReload()
	{
		loadFailed__();
	}
}