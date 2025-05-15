#pragma once
#include "Engine/AssetLoader.h"
#include <memory>

namespace Destiny
{
	class TextureLoader : public AssetLoader
	{
	public:
		TextureLoader();
		virtual ~TextureLoader();
	public:
		virtual void loadAsset(std::shared_ptr<Asset> asset) override;
	private:
		void loadFromPath(std::shared_ptr<Asset> asset);
		void loadFromMemory(std::shared_ptr<Asset> asset);
		void loadFromHDR(std::shared_ptr<Asset> asset);
	};
}