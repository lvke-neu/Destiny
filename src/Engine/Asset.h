#pragma once
#include <memory>

namespace Destiny
{
	class Object;
	class AssetLoader;
	class Asset : public std::enable_shared_from_this<Asset>
	{
	public:
		enum LoadingState
		{
			loading_state_pending,
			loading_state_failed,
			loading_state_succeeded
		};
	public:
		Asset();
		virtual ~Asset() = default;
	public:
		void						initialize(std::shared_ptr<AssetLoader> assetLoader, std::shared_ptr<Object> creationParam);
		std::shared_ptr<Object>		getCreationParam();
		void						load(int priority = 1);
		void						reload(int priority = 1);
		void						loadSucceeded__();
		void						loadFailed__();
		void						loadPending_();
		bool						isLoadingSucceed();
		bool						isLoadingFailed();
		bool						isLoadingPending();
	private:
		virtual void				doLoad();
		virtual void				doReload();
	protected:
		std::shared_ptr<AssetLoader>	m_assetLoader;
		std::shared_ptr<Object>			m_creationParam;
		LoadingState					m_state;
	};

	inline std::shared_ptr<Object> Asset::getCreationParam()
	{
		return m_creationParam;
	}

	inline bool Asset::isLoadingSucceed()
	{
		return m_state == loading_state_succeeded;
	}

	inline bool Asset::isLoadingFailed()
	{
		return m_state == loading_state_failed;
	}

	inline bool Asset::isLoadingPending()
	{
		return m_state == loading_state_pending;
	}

	inline void Asset::loadPending_()
	{
		m_state = loading_state_pending;;
	}

	inline void Asset::loadSucceeded__()
	{
		m_state = loading_state_succeeded;
	}

	inline void Asset::loadFailed__()
	{
		m_state = loading_state_failed;
	}
}