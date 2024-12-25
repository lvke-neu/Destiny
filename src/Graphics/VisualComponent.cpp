#include "VisualComponent.h"
#include "CameraComponent.h"
#include "VisualScene.h"
#include "RenderPass.h"
#include "Renderer.h"
#include "Engine/Scene.h"
#include "Engine/Node.h"

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
			m_visual->setComponent(shared_from_this());
		}
		onNodeTransformChanged();
	}

	void VisualComponent::onNodeTransformChanged()
	{
		if (!m_node || !m_visual)
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
		m_visual->setConstant("u_worldMatrix", worldMatrix);
	}

	void VisualComponent::onEnterScene()
	{
		auto visualScene = std::dynamic_pointer_cast<VisualScene>(m_scene);
		if (!visualScene || !visualScene->getCamera() || !visualScene->getCameraNode())
		{
			return;
		}
		onNodeTransformChanged();
		onCameraViewChanged(visualScene->getCameraNode()->get_transform().getInvTransposeWorldMatrix());
		onCameraProjChanged(
			DirectX::XMMatrixTranspose
			(
				DirectX::XMMatrixPerspectiveFovLH(visualScene->getCamera()->get_fovy(), visualScene->getCamera()->get_aspect(), visualScene->getCamera()->get_nearz(), visualScene->getCamera()->get_farz())
			),
			visualScene->getCamera()->get_viewportWidth(), visualScene->getCamera()->get_viewportHeight()
		);
	}

	void VisualComponent::onCameraViewChanged(const DirectX::XMMATRIX& cameraView)
	{
		if (!m_visual)
		{
			return;
		}
		
		m_visual->setConstant("g_view", cameraView);
	}

	void VisualComponent::onCameraProjChanged(const DirectX::XMMATRIX& cameraProj, float viewportWidth, float viewportHeight)
	{
		if (!m_visual)
		{
			return;
		}

		m_visual->setConstant("g_proj", cameraProj);
		m_visual->setConstant("g_viewportWidth", viewportWidth);
		m_visual->setConstant("g_rcpViewportWidth", 1.0f / viewportWidth);
		m_visual->setConstant("g_viewportHeight", viewportHeight);
		m_visual->setConstant("g_rcpViewportHeight", 1.0f / viewportHeight);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<VisualComponent>("VisualComponent")
			.constructor<>();
	}
}