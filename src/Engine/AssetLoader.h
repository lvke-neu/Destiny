#pragma once
#include <memory>
#include <mutex>

namespace Destiny
{
	class Asset;
	class BlobHolder;
	class AssetLoader : public std::enable_shared_from_this<AssetLoader>
	{
	public:
		AssetLoader();
		virtual ~AssetLoader() = default;
	public:
		virtual void loadAsset(std::shared_ptr<Asset> asset) = 0;
		virtual std::shared_ptr<Asset> createAsset(std::shared_ptr<BlobHolder> blobHolder) = 0;
		std::mutex m_mtx;
	};
}