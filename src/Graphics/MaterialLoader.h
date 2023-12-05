#pragma once
#include "Engine/AssetLoader.h"
#include <memory>

namespace Destiny
{
	class Material;
	class MaterialLoader : public AssetLoader, public std::enable_shared_from_this<MaterialLoader>
	{
	public:
		MaterialLoader();
		virtual ~MaterialLoader();
	public:
		virtual void loadAsset(std::shared_ptr<Asset> asset) override;
		std::shared_ptr<Material> createAsset();
	};
}