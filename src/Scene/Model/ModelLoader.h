#pragma once
#include "Engine/AssetLoader.h"
#include <memory>

struct aiNode;
namespace Destiny
{
	class Node;
	class ModelLoader : public AssetLoader
	{
	public:
		ModelLoader();
		virtual ~ModelLoader();
	public:
		virtual void loadAsset(std::shared_ptr<Asset> asset) override;
	private:
		std::shared_ptr<Node> copyTree(std::shared_ptr<Node> myNodeParent, aiNode* otherNode);
	};
}