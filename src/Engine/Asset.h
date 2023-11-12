#pragma once
#include <memory>

namespace Destiny
{
	class AssetLoader;
	class BlobHolder;
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
		void initialize(std::shared_ptr<AssetLoader> assetLoader, std::shared_ptr<BlobHolder> blobHolder);
		void load(int priority = 1);
		void loadSucceeded__();
		void loadFailed__();
		std::shared_ptr<BlobHolder> getBlobHolder() const;
	private:
		virtual void doLoad();
	private:
		std::shared_ptr<AssetLoader> m_assetLoader;
		std::shared_ptr<BlobHolder> m_blobHolder;
		LoadingState m_state;
	};

	inline std::shared_ptr<BlobHolder> Asset::getBlobHolder() const
	{
		return m_blobHolder;
	}
}