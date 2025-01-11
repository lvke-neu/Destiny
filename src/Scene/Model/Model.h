#pragma once
#include "Engine/Asset.h"
#include <unordered_set>
#include <DirectXCollision.h>

namespace Destiny
{
	class Node;
	class ModelLoader;
	class VisualComponent;
	class Model : public Asset
	{
		friend ModelLoader;
	public:
		Model();
		virtual ~Model();
	public:
		std::shared_ptr<Node>	getNode();
		void					onDataLoaded();
		void					setEnable(bool enable);
	public:
		static std::shared_ptr<ModelLoader> s_modelLoader;
		static std::shared_ptr<Model> Create(const char* path);
	private:
		std::shared_ptr<Node> m_node;
		std::unordered_set<std::shared_ptr<VisualComponent>> m_visualComponents;
		DirectX::BoundingBox m_mergedAABB;
	};

	inline std::shared_ptr<Node> Model::getNode()
	{
		return m_node;
	}
}