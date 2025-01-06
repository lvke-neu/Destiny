#include "VisualComponent.h"
#include "CameraComponent.h"
#include "VisualScene.h"
#include "RenderPass.h"
#include "Renderer.h"
#include "Engine/Scene.h"
#include "Engine/Node.h"
#include "Math/Math.h"

namespace Destiny
{
	VisualComponent::VisualComponent() : 
		m_visual(std::make_shared<Visual>())
	{

	}

	std::string VisualComponent::get_renderer()
	{
		return m_visual ? m_visual->get_renderer() : "";
	}

	void VisualComponent::set_renderer(std::string renderer)
	{
		if (m_visual)
		{
			m_visual->set_renderer(renderer);
			onEnterScene();
		}
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

		auto worldMatrix = m_node->getRootToThisWorldMatrix();
		m_visual->setConstant("u_worldMatrix", XMMatrixTranspose(worldMatrix));
		worldMatrix.r[3] = DirectX::g_XMIdentityR3;
		worldMatrix = XMMatrixTranspose(XMMatrixInverse(nullptr, worldMatrix));
		m_visual->setConstant("u_worldInvTransposeMatrix", XMMatrixTranspose(worldMatrix));
	}

	void VisualComponent::onEnterScene()
	{
		auto visualScene = std::dynamic_pointer_cast<VisualScene>(m_scene);
		if (!visualScene || !visualScene->getCamera() || !visualScene->getCameraNode())
		{
			return;
		}
		onNodeTransformChanged();
		onCameraViewChanged(visualScene->getCameraNode()->get_transform().getInvTransposeWorldMatrix(), visualScene->getCameraNode()->get_transform().get_translation());
		onCameraProjChanged(
			DirectX::XMMatrixTranspose
			(
				DirectX::XMMatrixPerspectiveFovLH(visualScene->getCamera()->get_fovy() * Math::DEG2RAD, visualScene->getCamera()->get_aspect(), visualScene->getCamera()->get_nearz(), visualScene->getCamera()->get_farz())
			),
			visualScene->getCamera()->get_viewportWidth(), visualScene->getCamera()->get_viewportHeight()
		);

		onLightDirectionChanged(visualScene->getDirectionLightNode()->get_transform().get_rotation());
	}

	void VisualComponent::onCameraViewChanged(const DirectX::XMMATRIX& cameraView, const DirectX::XMFLOAT3& eyePosition)
	{
		if (!m_visual)
		{
			return;
		}
		
		m_visual->setConstant("g_view", cameraView);
		m_visual->setConstant("g_eyePosition", eyePosition);
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

	void VisualComponent::onLightDirectionChanged(const DirectX::XMFLOAT3& direction)
	{
		if (!m_visual)
		{
			return;
		}
		m_visual->setConstant("g_direction", direction);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<VisualComponent>("VisualComponent")
			.constructor<>()
			.property("renderer", &VisualComponent::get_renderer, &VisualComponent::set_renderer);
	}
}