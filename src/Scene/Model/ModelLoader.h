//#pragma once
//#include "Engine/AssetLoader.h"
//#include <memory>
//
//struct aiScene;
//struct aiNode;
//struct aiMesh;
//struct aiMaterial;
//namespace Destiny
//{
//	class Node;
//	class RenderPass;
//	class Mesh;
//	class Material;
//	class Model;
//	class Texture;
//	class ModelLoader : public AssetLoader
//	{
//	public:
//		ModelLoader();
//		virtual ~ModelLoader();
//	public:
//		virtual void loadAsset(std::shared_ptr<Asset> asset) override;
//	private:
//		static std::shared_ptr<Texture>		s_albedo;
//		static std::shared_ptr<Texture>		s_normal;
//		static std::shared_ptr<Texture>		s_metallic;
//		static std::shared_ptr<Texture>		s_roughness;
//		static std::shared_ptr<Texture>		s_ao;
//	private:
//		std::shared_ptr<Node>			copyTree(const aiScene* otherScene, std::shared_ptr<Node> myNodeParent, aiNode* otherNode, std::shared_ptr<Model> model);
//		std::shared_ptr<RenderPass>		getRenderPass();
//		std::shared_ptr<Mesh>			getMesh(aiMesh* otherMesh, std::shared_ptr<Model> model);
//		std::shared_ptr<Material>		getMaterial(aiMaterial* otherMaterial);
//	};
//}