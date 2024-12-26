#pragma once
#include <memory>

namespace Destiny
{
	class Asset;
	class AssetLoader
	{
	public:
		AssetLoader() = default;
		virtual ~AssetLoader() = default;
	public:
		virtual void loadAsset(std::shared_ptr<Asset> asset) = 0;
	};
}