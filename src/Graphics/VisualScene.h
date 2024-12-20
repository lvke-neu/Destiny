#pragma once
#include "Engine/Scene.h"

namespace Destiny
{
	class CameraComponent;
	class CameraController;
	class VisualScene : public Scene
	{
	public:
		VisualScene(const std::string& name);
		virtual ~VisualScene() = default;
	public:
		virtual void initialize() override;
		virtual void uninitialize() override;
		virtual void update(float deltaTime) override;
		virtual void onCull() override;
	public:
		std::shared_ptr<CameraComponent>	getCamera();
		std::shared_ptr<CameraController>	getCameraController();
		std::shared_ptr<Node>				getCameraNode();
	protected:
		std::shared_ptr<CameraComponent>	m_camera;
		std::shared_ptr<CameraController>	m_cameraController;
		std::shared_ptr<Node>				m_cameraNode;
	};

	inline std::shared_ptr<CameraComponent> VisualScene::getCamera()
	{
		return m_camera;
	}

	inline std::shared_ptr<CameraController> VisualScene::getCameraController()
	{
		return m_cameraController;
	}

	inline std::shared_ptr<Node> VisualScene::getCameraNode()
	{
		return m_cameraNode;
	}
}