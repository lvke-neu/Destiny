#include "VisualComponent.h"
#include "Scene.h"
#include "Node.h"
#include "CameraComponent.h"
#include "Graphics/RenderPass.h"
#include "Graphics/Renderer.h"

namespace Destiny
{
	VisualComponent::VisualComponent() : 
		m_visual(std::make_shared<Visual>())
	{

	}

	void VisualComponent::onAddToNode()
	{
		onNodeTransformChanged();
	}

	void VisualComponent::onNodeTransformChanged()
	{
		if (!m_node || !m_visual || !m_visual->getRenderPass() || !m_visual->getRenderPass()->getRenderer())
		{
			return;
		}

		m_visual->getRenderPass()->getRenderer()->
			setConstant("u_worldMatrix", m_node->get_transform().getTransposeWorldMatrix());
	}

	void VisualComponent::onEnterScene()
	{
		if (!m_scene || !m_scene->getSceneCameraNode() || !m_scene->getSceneCamera())
		{
			return;
		}
		
		onCameraViewChanged(m_scene->getSceneCameraNode()->get_transform().getInvTransposeWorldMatrix());
		auto camera = m_scene->getSceneCamera();
		onCameraProjChanged(DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(camera->get_fovy(), camera->get_aspect(), camera->get_nearz(), camera->get_farz())));
	}

	void VisualComponent::onCameraViewChanged(const DirectX::XMMATRIX& cameraView)
	{
		if (!m_visual || !m_visual->getRenderPass() || !m_visual->getRenderPass()->getRenderer())
		{
			return;
		}
		
		m_visual->getRenderPass()->getRenderer()->setConstant("g_view", cameraView);
	}

	void VisualComponent::onCameraProjChanged(const DirectX::XMMATRIX& cameraProj)
	{
		if (!m_visual || !m_visual->getRenderPass() || !m_visual->getRenderPass()->getRenderer())
		{
			return;
		}

		m_visual->getRenderPass()->getRenderer()->setConstant("g_proj", cameraProj);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<VisualComponent>("VisualComponent")
			.constructor<>();
	}
}