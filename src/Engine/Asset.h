#pragma once
#include <memory>
#include <atomic>

namespace Destiny
{
	class Object;
	class AssetLoader;
	class Asset : public std::enable_shared_from_this<Asset>
	{
	public:
		enum  class LoadingState
		{
			loading_state_pending,
			loading_state_failed,
			loading_state_succeeded,
			loading_state_start
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
		void						loadStart__();
		bool						isLoadingSucceed();
		bool						isLoadingFailed();
		bool						isLoadingPending();
		bool						isLoadingStart();
	private:
		virtual void				doLoad();
		virtual void				doReload();
	protected:
		std::shared_ptr<AssetLoader>	m_assetLoader;
		std::shared_ptr<Object>			m_creationParam;
		std::atomic<LoadingState>		m_state;
	};

	inline std::shared_ptr<Object> Asset::getCreationParam()
	{
		return m_creationParam;
	}

	inline bool Asset::isLoadingSucceed()
	{
		return m_state == LoadingState::loading_state_succeeded;
	}

	inline bool Asset::isLoadingFailed()
	{
		return m_state == LoadingState::loading_state_failed;
	}

	inline bool Asset::isLoadingPending()
	{
		return m_state == LoadingState::loading_state_pending;
	}

	inline bool Asset::isLoadingStart()
	{
		return m_state == LoadingState::loading_state_start;
	}

	inline void Asset::loadPending_()
	{
		m_state = LoadingState::loading_state_pending;;
	}

	inline void Asset::loadSucceeded__()
	{
		m_state = LoadingState::loading_state_succeeded;
	}

	inline void Asset::loadFailed__()
	{
		m_state = LoadingState::loading_state_failed;
	}

	inline void Asset::loadStart__()
	{
		m_state = LoadingState::loading_state_start;
	}
}