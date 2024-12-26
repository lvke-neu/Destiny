#include "CameraComponent.h"
#include "VisualComponent.h"
#include "Graphics/VisualScene.h"
#include "Engine/EventSystem.h"


namespace Destiny
{
	CameraComponent::CameraComponent() :
		m_fovy(45.0f),
		m_aspect(1.0f),
		m_nearz(0.1f),
		m_farz(1000.0f),
		m_viewportWidth(1.0f),
		m_viewportHeight(1.0f)
	{
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&CameraComponent::onWindowResize, this, std::placeholders::_1));
	}

	CameraComponent::~CameraComponent()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&CameraComponent::onWindowResize, this, std::placeholders::_1));
	}

	void CameraComponent::onEnterScene()
	{
		traversalViewChanged(m_scene);
	}

	void CameraComponent::onNodeTransformChanged()
	{
		traversalViewChanged(m_scene);
	}

	DirectX::XMMATRIX CameraComponent::getProjectionMatrix()
	{
		return DirectX::XMMatrixPerspectiveFovLH(m_fovy, m_aspect, m_nearz, m_farz);
	}

	void CameraComponent::onWindowResize(void* data)
	{
		WindowResizeData ws = *(WindowResizeData*)(data);
		m_viewportWidth = (float)ws.width;
		m_viewportHeight = (float)ws.height;
		m_aspect = m_viewportWidth / m_viewportHeight;
		traversalProjChanged(m_scene);
	}

	void CameraComponent::traversalViewChanged(std::shared_ptr<Node> node)
	{
		if (!node)
		{
			return;
		}

		for (const auto& component : node->getComponents())
		{
			auto visualComponent = std::dynamic_pointer_cast<VisualComponent>(component);
			if (visualComponent)
			{
				if (m_node)
				{
					visualComponent->onCameraViewChanged(m_node->get_transform().getInvTransposeWorldMatrix(), m_node->get_transform().get_translation());
				}	
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			traversalViewChanged(childNode);
		}
	}

	void CameraComponent::traversalProjChanged(std::shared_ptr<Node> node)
	{
		if (!node)
		{
			return;
		}

		for (const auto& component : node->getComponents())
		{
			auto visualComponent = std::dynamic_pointer_cast<VisualComponent>(component);
			if (visualComponent)
			{
				visualComponent->onCameraProjChanged(DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(m_fovy, m_aspect, m_nearz, m_farz)), m_viewportWidth, m_viewportHeight);
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			traversalProjChanged(childNode);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<CameraComponent>("CameraComponent")
			.constructor<>();
	}
}