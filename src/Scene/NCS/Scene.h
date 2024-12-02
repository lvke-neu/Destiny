#pragma once
#include "Node.h"

namespace Destiny
{
	class CameraComponent;
	class CameraController;
	class Scene : public Node
	{
	public:
		Scene(const std::string& name);
		virtual ~Scene() = default;
	public:
		virtual void initialize();
		virtual void uninitialize() = 0;
		virtual void update(float deltaTime) = 0;
		virtual void onCull() = 0;
	public:
		std::shared_ptr<CameraComponent>	getCamera();
		std::shared_ptr<CameraController>	getCameraController();
		std::shared_ptr<Node>				getCameraNode();
	protected:
		std::shared_ptr<CameraComponent>	m_camera;
		std::shared_ptr<CameraController>	m_cameraController;
		std::shared_ptr<Node>				m_cameraNode;
	};

	inline std::shared_ptr<CameraComponent> Scene::getCamera()
	{
		return m_camera;
	}

	inline std::shared_ptr<CameraController> Scene::getCameraController()
	{
		return m_cameraController;
	}

	inline std::shared_ptr<Node> Scene::getCameraNode()
	{
		return m_cameraNode;
	}
}