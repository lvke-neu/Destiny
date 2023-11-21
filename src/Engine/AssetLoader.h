#pragma once
#include <mutex>
namespace Destiny
{
	class Asset;
	class BlobHolder;
	class AssetLoader
	{
	public:
		AssetLoader();
		virtual ~AssetLoader() = default;
	public:
		virtual void loadAsset(std::shared_ptr<Asset> asset) = 0;
		std::mutex m_mtx;
	};
}