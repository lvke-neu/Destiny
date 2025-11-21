#include "CameraComponent.h"
#include "VisualComponent.h"
#include "Graphics/VisualScene.h"
#include "Engine/EventSystem.h"
#include "RenderSystem.h"
#include "DeferredOpaquePipeline.h"
#include "Math/Math.h"
#include "BindRenderTargets.h"
#include "Graphics/Renderer.h"

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

	void CameraComponent::set_fovy(float fovy)
	{
		m_fovy = fovy;
		if (m_fovy < Math::FLT_THRESHOLD)
		{
			m_fovy = 45.0f;
		}
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene.lock());
		setRendererConstant();
	}

	void CameraComponent::set_aspect(float aspect)
	{
		m_aspect = aspect;
		if (m_aspect < Math::FLT_THRESHOLD)
		{
			m_aspect = 1.0f;
		}
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene.lock());
		setRendererConstant();
	}

	void CameraComponent::set_nearz(float nearz)
	{
		m_nearz = nearz;
		if (m_nearz < Math::FLT_THRESHOLD)
		{
			m_nearz = 0.1f;
		}
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene.lock());
		setRendererConstant();
	}

	void CameraComponent::set_farz(float farz)
	{
		m_farz = farz;
		if (m_farz < Math::FLT_THRESHOLD)
		{
			m_farz = 1000.0f;
		}
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene.lock());
		setRendererConstant();
	}

	void CameraComponent::onEnterScene()
	{
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto bindRenderTargets = renderSystem->m_bindRenderTargets;
		if (bindRenderTargets)
		{
			auto viewPort = bindRenderTargets->getViewPort(0);
			if (viewPort)
			{
				m_viewportWidth = viewPort->Width;
				m_viewportHeight = viewPort->Height;
				m_aspect = m_viewportWidth / m_viewportHeight;
			}
		}
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene.lock());
		setRendererConstant();
	}

	void CameraComponent::onNodeTransformChanged()
	{
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene.lock());
		setRendererConstant();
	}

	DirectX::XMMATRIX CameraComponent::getProjectionMatrix()
	{
		return DirectX::XMMatrixPerspectiveFovLH(m_fovy * Math::DEG2RAD, m_aspect, m_nearz, m_farz);
	}

	void CameraComponent::onWindowResize(void* data)
	{
		WindowResizeData ws = *(WindowResizeData*)(data);
		m_viewportWidth = (float)ws.width;
		m_viewportHeight = (float)ws.height;
		m_aspect = m_viewportWidth / m_viewportHeight;
		if (m_aspect < Math::FLT_THRESHOLD)
		{
			m_aspect = 1.0f;
		}
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene.lock());
		setRendererConstant();
	}


	void CameraComponent::notifyVisualRendererConstantChanged(std::shared_ptr<Node> node)
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
				visualComponent->onRendererConstantChanged();
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			notifyVisualRendererConstantChanged(childNode);
		}
	}

	void CameraComponent::setRendererConstant()
	{
		if (!m_node.lock())
		{
			return;
		}

		const auto& cameraViewMatrix = m_node.lock()->getInvTransposeWorldMatrix();
		const auto& cameraPos = m_node.lock()->get_translation();
		const auto& cameraProjMaTrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(m_fovy * Math::DEG2RAD, m_aspect, m_nearz, m_farz));

		Renderer::SetConstant("g_view", cameraViewMatrix);
		Renderer::SetConstant("g_eyePosition", cameraPos);

		Renderer::SetConstant("g_proj", cameraProjMaTrix);
		Renderer::SetConstant("g_viewportWidth", m_viewportWidth);
		Renderer::SetConstant("g_rcpViewportWidth", 1.0f / m_viewportWidth);
		Renderer::SetConstant("g_viewportHeight", m_viewportHeight);
		Renderer::SetConstant("g_rcpViewportHeight", 1.0f / m_viewportHeight);
		Renderer::SetConstant("g_nearPlane", m_nearz);
		Renderer::SetConstant("g_farPlane", m_farz);	
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<CameraComponent>("CameraComponent")
			.constructor<>()
			.property("fovy", &CameraComponent::get_fovy, &CameraComponent::set_fovy)
			.property("aspect", &CameraComponent::get_aspect, &CameraComponent::set_aspect)
			.property("nearz", &CameraComponent::get_nearz, &CameraComponent::set_nearz)
			.property("farz", &CameraComponent::get_farz, &CameraComponent::set_farz);
	}
}