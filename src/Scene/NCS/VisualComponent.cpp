#include "VisualComponent.h"
#include "VisualScene.h"
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
		auto visualScene = std::dynamic_pointer_cast<VisualScene>(m_scene);

		if (!visualScene)
		{
			return;
		}
		
		auto cameraNode = visualScene->findCameraNode();
		if (!cameraNode || cameraNode->getComponents().empty())
		{
			return;
		}

		auto camera = std::dynamic_pointer_cast<CameraComponent>(cameraNode->getComponents()[0]);
		if (!camera)
		{
			return;
		}

		onCameraViewChanged(cameraNode->get_transform().getInvTransposeWorldMatrix());
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