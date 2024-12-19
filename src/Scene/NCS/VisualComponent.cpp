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
		if (m_visual)
		{
			m_visual->bindComponent(shared_from_this());
		}
		onNodeTransformChanged();
	}

	void VisualComponent::onNodeTransformChanged()
	{
		if (!m_node || !m_visual || !m_visual->getRenderPass() || !m_visual->getRenderPass()->getRenderer())
		{
			return;
		}

		auto tmpNode = m_node;
		auto scaleMatrix = DirectX::XMMatrixIdentity();
		auto rotationMatrix = DirectX::XMMatrixIdentity();
		auto translationMatrix = DirectX::XMMatrixIdentity();
		while (tmpNode)
		{
			scaleMatrix = tmpNode->get_transform().getScaleMatrix() * scaleMatrix;
			rotationMatrix = tmpNode->get_transform().getRotationMatrix() * rotationMatrix;
			translationMatrix = tmpNode->get_transform().getTranslationMatrix() * translationMatrix;
			tmpNode = tmpNode->getParent();
		}
		auto worldMatrix = DirectX::XMMatrixTranspose(scaleMatrix * rotationMatrix * translationMatrix);
		m_visual->getRenderPass()->getRenderer()->
			setConstant("u_worldMatrix", worldMatrix);
	}

	void VisualComponent::onEnterScene()
	{
		if (!m_scene || !m_scene->getCamera() || !m_scene->getCameraNode())
		{
			return;
		}
		
		onCameraViewChanged(m_scene->getCameraNode()->get_transform().getInvTransposeWorldMatrix());
		onCameraProjChanged(DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(m_scene->getCamera()->get_fovy(), m_scene->getCamera()->get_aspect(), m_scene->getCamera()->get_nearz(), m_scene->getCamera()->get_farz())),
			m_scene->getCamera()->get_viewportWidth(), m_scene->getCamera()->get_viewportHeight());
	}

	void VisualComponent::onCameraViewChanged(const DirectX::XMMATRIX& cameraView)
	{
		if (!m_visual || !m_visual->getRenderPass() || !m_visual->getRenderPass()->getRenderer())
		{
			return;
		}
		
		m_visual->getRenderPass()->getRenderer()->setConstant("g_view", cameraView);
	}

	void VisualComponent::onCameraProjChanged(const DirectX::XMMATRIX& cameraProj, float viewportWidth, float viewportHeight)
	{
		if (!m_visual || !m_visual->getRenderPass() || !m_visual->getRenderPass()->getRenderer())
		{
			return;
		}

		m_visual->getRenderPass()->getRenderer()->setConstant("g_proj", cameraProj);
		m_visual->getRenderPass()->getRenderer()->setConstant("g_viewportWidth", viewportWidth);
		m_visual->getRenderPass()->getRenderer()->setConstant("g_rcpViewportWidth", 1.0f / viewportWidth);
		m_visual->getRenderPass()->getRenderer()->setConstant("g_viewportHeight", viewportHeight);
		m_visual->getRenderPass()->getRenderer()->setConstant("g_rcpViewportHeight", 1.0f / viewportHeight);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<VisualComponent>("VisualComponent")
			.constructor<>();
	}
}