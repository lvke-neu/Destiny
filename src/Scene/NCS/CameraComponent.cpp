#include "CameraComponent.h"
#include "Engine/EventSystem.h"
#include "Scene.h"
#include "VisualComponent.h"

namespace Destiny
{
	CameraComponent::CameraComponent() :
		m_fovy(60.0f),
		m_aspect(1.0f),
		m_nearz(1.0f),
		m_farz(1000.0f)
	{
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&CameraComponent::onWindowResize, this, std::placeholders::_1));
	}

	CameraComponent::~CameraComponent()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&CameraComponent::onWindowResize, this, std::placeholders::_1));
	}

	void CameraComponent::onEnterScene(std::shared_ptr<Scene> scene)
	{
		m_scene = scene;
	}

	void CameraComponent::onNodeTransformChanged(const Transform& transform)
	{
		bfsNotifyViewChanged(m_scene, transform);
	}

	void CameraComponent::onWindowResize(void* data)
	{
		WindowResizeData ws = *(WindowResizeData*)(data);
		m_aspect = static_cast<float>(ws.width) / ws.height;
		bfsNotifyProjChanged(m_scene);
	}

	void CameraComponent::bfsNotifyViewChanged(std::shared_ptr<Node> node, const Transform& transform)
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
				visualComponent->onCameraViewChanged(transform.getInvTransposeWorldMatrix());
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			bfsNotifyViewChanged(childNode, transform);
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