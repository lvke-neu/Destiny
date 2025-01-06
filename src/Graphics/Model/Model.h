#pragma once
#include "Engine/Asset.h"
#include <unordered_map>
#include <DirectXCollision.h>

namespace Destiny
{
	class Node;
	class ModelLoader;
	class VisualComponent;
	class Material;
	class Model : public Asset
	{
		friend ModelLoader;
	public:
		Model();
		virtual ~Model();
	public:
		std::shared_ptr<Node>	getNode();
		void					loadVisuals();
		void					setEnable(bool enable);
	public:
		static std::shared_ptr<ModelLoader> s_modelLoader;
		static std::shared_ptr<Model> Create(const char* path);
	private:
		std::shared_ptr<Node> m_node;
		std::unordered_map<std::shared_ptr<VisualComponent>, std::shared_ptr<Material>> m_visuals;
		DirectX::BoundingBox m_mergedAABB;
	};

	inline std::shared_ptr<Node> Model::getNode()
	{
		return m_node;
	}
}