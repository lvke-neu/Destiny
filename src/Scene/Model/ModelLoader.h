#pragma once
#include "Engine/AssetLoader.h"
#include <memory>

namespace Destiny
{
	class ModelLoader : public AssetLoader
	{
	public:
		ModelLoader();
		virtual ~ModelLoader();
	public:
		virtual void loadAsset(std::shared_ptr<Asset> asset) override;
	};
}