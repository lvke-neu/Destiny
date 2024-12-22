#pragma once
#include "Engine/Asset.h"

namespace Destiny
{
	class Node;
	class ModelLoader;
	class Model : public Asset
	{
		friend ModelLoader;
	public:
		Model();
		virtual ~Model();
	public:
		std::shared_ptr<Node> getNode();
	public:
		static std::shared_ptr<ModelLoader> s_modelLoader;
		static std::shared_ptr<Model> Create(const char* path);
	private:
		std::shared_ptr<Node> m_node;
	};

	inline std::shared_ptr<Node> Model::getNode()
	{
		return m_node;
	}
}