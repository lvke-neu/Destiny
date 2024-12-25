#pragma once
#include "Engine/AssetLoader.h"
#include <memory>
#include <DirectXCollision.h>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
namespace Destiny
{
	class Node;
	class RenderPass;
	class Mesh;
	class Material;
	class ModelLoader : public AssetLoader
	{
	public:
		ModelLoader();
		virtual ~ModelLoader();
	public:
		virtual void loadAsset(std::shared_ptr<Asset> asset) override;
	private:
		std::shared_ptr<Node>			copyTree(const aiScene* otherScene, std::shared_ptr<Node> myNodeParent, aiNode* otherNode, DirectX::BoundingBox& mergedAABB);
		std::shared_ptr<RenderPass>		getRenderPass();
		std::shared_ptr<Mesh>			getMesh(aiMesh* otherMesh, DirectX::BoundingBox& mergedAABB);
		std::shared_ptr<Material>		getMaterial(aiMaterial* otherMaterial);
	};
}