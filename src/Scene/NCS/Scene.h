#pragma once
#include "Node.h"

namespace Destiny
{
	class CameraComponent;
	class Node;
	class Scene : public Node
	{
	public:
		Scene();
		~Scene();
	public:
		void initialize();
		void uninitialize();
		void update(float deltaTime);
	public:
		std::shared_ptr<CameraComponent> getSceneCamera();
		std::shared_ptr<Node> getSceneCameraNode();
	private:
		std::shared_ptr<CameraComponent> m_camera;
		std::shared_ptr<Node> m_cameraNode;
	};

	inline std::shared_ptr<CameraComponent> Scene::getSceneCamera()
	{
		return m_camera;
	}

	inline std::shared_ptr<Node> Scene::getSceneCameraNode()
	{
		return m_cameraNode;
	}
}