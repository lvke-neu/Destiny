#pragma once
#include "Engine/AssetLoader.h"
#include <memory>

namespace Destiny
{
	class TextureLoader : public AssetLoader, public std::enable_shared_from_this<TextureLoader>
	{
	public:
		TextureLoader();
		virtual ~TextureLoader();
	public:
		virtual void loadAsset(std::shared_ptr<Asset> asset) override;
	};
}