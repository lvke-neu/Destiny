#pragma once
#include "Engine/Scene.h"

namespace Destiny
{
	class  CameraComponent;
	class  CameraController;
	class  DirectionLightComponent;
	class  BindRenderTargets;
	class  ClearRenderTargets;
	struct Pre_Post_Process_Command;
	class VisualScene : public Scene
	{
		RTTR_ENABLE(Scene);
	public:
		VisualScene(const std::string& name);
		virtual ~VisualScene() = default;
	public:
		GET_CLASS_NAME(VisualScene);
	public:
		virtual void								initialize() override;
		virtual void								uninitialize() override;
		virtual void								update(float deltaTime) override;
		virtual void								onCull() override;
	public:
		std::shared_ptr<CameraComponent>			getCamera();
		std::shared_ptr<CameraController>			getCameraController();
		std::shared_ptr<Node>						getCameraNode();

		std::shared_ptr<DirectionLightComponent>	getDirectionLight();
		std::shared_ptr<Node>						getDirectionLightNode();
	public:
		std::shared_ptr<BindRenderTargets>			m_bindRenderTargets;
	private:
		std::shared_ptr<Pre_Post_Process_Command>	m_pre_post_process_command;
		std::shared_ptr<ClearRenderTargets>			m_clearRenderTargets;
	protected:
		std::shared_ptr<CameraComponent>			m_camera;
		std::shared_ptr<CameraController>			m_cameraController;
		std::shared_ptr<Node>						m_cameraNode;

		std::shared_ptr<DirectionLightComponent>	m_directionLight;
		std::shared_ptr<Node>						m_directionLightNode;
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

	inline std::shared_ptr<DirectionLightComponent> VisualScene::getDirectionLight()
	{
		return m_directionLight;
	}
	
	inline std::shared_ptr<Node> VisualScene::getDirectionLightNode()
	{
		return m_directionLightNode;
	}
}