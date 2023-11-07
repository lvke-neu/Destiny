#pragma once
#include <memory>

namespace Destiny
{
	class Asset;
	class VirtualDestructor;
	class AssetLoader
	{
	public:
		virtual ~AssetLoader() = default;
	public:
		virtual void createAsset(std::shared_ptr<VirtualDestructor> creationParam) = 0;
		virtual void loadAsset(std::shared_ptr<Asset> asset) = 0;
		virtual void modifyAsset(std::shared_ptr<Asset> asset) = 0;
	};
}