#pragma once
#include <memory>

namespace Destiny
{
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
		void initialize(std::shared_ptr<AssetLoader> assetLoader);
		void load(int priority = 1);
		void loadSucceeded__();
		void loadFailed__();
		void loadPending_();
		bool isLoadingFinished();
		bool isLoadingSucceed();
		bool isLoadingPending();
	private:
		virtual void doLoad();
	protected:
		std::shared_ptr<AssetLoader>	m_assetLoader;
		LoadingState					m_state;
	};

	inline bool Asset::isLoadingSucceed()
	{
		return m_state == loading_state_succeeded;
	}

	inline bool Asset::isLoadingPending()
	{
		return m_state == loading_state_pending;
	}

	inline void Asset::loadPending_()
	{
		m_state = loading_state_pending;;
	}
}