#pragma once
#include <memory>

namespace Destiny
{
	class Node3D;
	class Scene3D
	{
	public:
		Scene3D();
		std::shared_ptr<Node3D> getRootNode();
	private:
		std::shared_ptr<Node3D> m_root;
	};

	inline std::shared_ptr<Node3D> Scene3D::getRootNode()
	{
		return m_root;
	}
}