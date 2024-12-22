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
	};
}