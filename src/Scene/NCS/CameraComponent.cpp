#include "CameraComponent.h"
#include "Engine/EventSystem.h"
#include "Scene.h"
#include "VisualComponent.h"

namespace Destiny
{
	CameraComponent::CameraComponent() :
		m_fovy(45.0f),
		m_aspect(1.0f),
		m_nearz(0.1f),
		m_farz(1000.0f)
	{
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&CameraComponent::onWindowResize, this, std::placeholders::_1));
	}

	CameraComponent::~CameraComponent()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&CameraComponent::onWindowResize, this, std::placeholders::_1));
	}

	void CameraComponent::onEnterScene()
	{
		bfsNotifyViewChanged(m_scene);
	}

	void CameraComponent::onNodeTransformChanged()
	{
		bfsNotifyViewChanged(m_scene);
	}

	void CameraComponent::onWindowResize(void* data)
	{
		WindowResizeData ws = *(WindowResizeData*)(data);
		m_aspect = static_cast<float>(ws.width) / ws.height;
		bfsNotifyProjChanged(m_scene);
	}

	void CameraComponent::bfsNotifyViewChanged(std::shared_ptr<Node> node)
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
					visualComponent->onCameraViewChanged(m_node->get_transform().getInvTransposeWorldMatrix());
				}	
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			bfsNotifyViewChanged(childNode);
		}
	}

	void CameraComponent::bfsNotifyProjChanged(std::shared_ptr<Node> node)
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
				visualComponent->onCameraProjChanged(DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(m_fovy, m_aspect, m_nearz, m_farz)));
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			bfsNotifyProjChanged(childNode);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<CameraComponent>("CameraComponent")
			.constructor<>();
	}
}