#pragma once
#include "Engine/AssetLoader.h"

namespace Destiny
{
	class VertexBufferLoader : public AssetLoader
	{
	public:
		virtual void loadAsset(std::shared_ptr<Asset> asset) override;
		virtual std::shared_ptr<Asset> createAsset(std::shared_ptr<BlobHolder> blobHolder) override;
	};
}