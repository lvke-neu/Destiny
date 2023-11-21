#pragma once
#include "Engine/AssetLoader.h"
#include <memory>

namespace Destiny
{
	class Texture;
	class TextureLoader_color : public AssetLoader, public std::enable_shared_from_this<TextureLoader_color>
	{
	public:
		TextureLoader_color();
		virtual ~TextureLoader_color();
	public:
		virtual void loadAsset(std::shared_ptr<Asset> asset) override;
		std::shared_ptr<Texture> createAsset(std::shared_ptr<BlobHolder> blobHolder);
	};
}