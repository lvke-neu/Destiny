#pragma once
#include <memory>

namespace Destiny
{
	class Node3D;
	class Scene3D : public std::enable_shared_from_this<Scene3D>
	{
	public:
		Scene3D();
	public:
		void initialize();
		std::shared_ptr<Node3D> getRootNode();
		std::shared_ptr<Node3D> getCameraNode();
	private:
		std::shared_ptr<Node3D> m_rootNode;
		std::shared_ptr<Node3D> m_cameraNode;
	};

	inline std::shared_ptr<Node3D> Scene3D::getRootNode()
	{
		return m_rootNode;
	}

	inline std::shared_ptr<Node3D> Scene3D::getCameraNode()
	{
		return m_cameraNode;
	}
}