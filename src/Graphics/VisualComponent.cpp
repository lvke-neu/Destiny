#include "VisualComponent.h"
#include "CameraComponent.h"
#include "DirectionLightComponent.h"
#include "PointLightComponent.h"
#include "VisualScene.h"
#include "RenderPass.h"
#include "Renderer.h"
#include "Engine/Scene.h"
#include "Engine/Node.h"
#include "Math/Math.h"

namespace Destiny
{
	VisualComponent::VisualComponent() : 
		m_visual(std::make_shared<Visual>()),
		m_shadowVisual(std::make_shared<Visual>())
	{
		m_shadowVisual->set_visualCategory(VisualCategory::RenderToShadowMap);
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

	void VisualComponent::set_shadowRenderer(std::string renderer)
	{
		if (m_shadowVisual)
		{
			m_shadowVisual->set_renderer(renderer);
		}
	}

	RendererCategory VisualComponent::get_rendererCategory()
	{
		return m_visual ? m_visual->get_rendererCategory() : RendererCategory::None;
	}

	void VisualComponent::set_rendererCategory(RendererCategory rendererCategory)
	{
		if (m_visual)
		{
			m_visual->set_rendererCategory(rendererCategory);
		}
	}

	D3D11_RASTERIZER_DESC	VisualComponent::get_rasterizerDesc()
	{
		if (m_visual)
		{
			return m_visual->get_rasterizerDesc();
		}
		return D3D11_RASTERIZER_DESC();
	}

	void VisualComponent::set_rasterizerDesc(D3D11_RASTERIZER_DESC desc)
	{
		if (m_visual)
		{
			m_visual->set_rasterizerDesc(desc);
		}
	}

	D3D11_DEPTH_STENCIL_DESC VisualComponent::get_depthStencilDesc()
	{
		if (m_visual)
		{
			return m_visual->get_depthStencilDesc();
		}
		return D3D11_DEPTH_STENCIL_DESC();
	}

	void VisualComponent::set_depthStencilDesc(D3D11_DEPTH_STENCIL_DESC desc)
	{
		if (m_visual)
		{
			m_visual->set_depthStencilDesc(desc);
		}
	}

	D3D11_BLEND_DESC VisualComponent::get_blendDesc()
	{
		if (m_visual)
		{
			return m_visual->get_blendDesc();
		}
		return D3D11_BLEND_DESC();
	}

	void VisualComponent::set_blendDesc(D3D11_BLEND_DESC desc)
	{
		if (m_visual)
		{
			return m_visual->set_blendDesc(desc);
		}
	}

	void VisualComponent::reCompileShader()
	{
		if (m_visual)
		{
			m_visual->reCompileShader();
		}
	}

	std::shared_ptr<Material> VisualComponent::get_material()
	{
		if (m_visual)
		{
			return m_visual->get_material();
		}
		return nullptr;
	}

	void VisualComponent::set_material(std::shared_ptr<Material> material)
	{
		if (m_visual)
		{
			m_visual->set_material(material);
		}
	}

	bool VisualComponent::get_enableShadow()
	{
		if (m_shadowVisual)
		{
			return m_shadowVisual->get_enableShadow();
		}
		return false;
	}

	void VisualComponent::set_enableShadow(bool enableShadow)
	{
		if (m_shadowVisual)
		{
			m_shadowVisual->set_enableShadow(enableShadow);
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
		if (!m_node.lock() || !m_visual)
		{
			return;
		}

		auto worldMatrix = m_node.lock()->getRootToThisWorldMatrix();
		m_visual->setConstant("u_worldMatrix", XMMatrixTranspose(worldMatrix));
		m_shadowVisual->setConstant("u_worldMatrix", XMMatrixTranspose(worldMatrix));
		worldMatrix.r[3] = DirectX::g_XMIdentityR3;
		worldMatrix = XMMatrixTranspose(XMMatrixInverse(nullptr, worldMatrix));
		m_visual->setConstant("u_worldInvTransposeMatrix", XMMatrixTranspose(worldMatrix));
		m_shadowVisual->setConstant("u_worldInvTransposeMatrix", XMMatrixTranspose(worldMatrix));

		onPropertyChanged("NodeTransform");
	}

	void VisualComponent::onEnterScene()
	{
		auto visualScene = std::dynamic_pointer_cast<VisualScene>(m_scene.lock());
		if (!visualScene || !visualScene->getCamera() || !visualScene->getCameraNode())
		{
			return;
		}
		onNodeTransformChanged();
		onRendererConstantChanged();
	}

	void VisualComponent::onRendererConstantChanged()
	{
		if (m_visual)
		{
			m_visual->setRendererConstantChanged();
		}
		if (m_shadowVisual)
		{
			m_shadowVisual->setRendererConstantChanged();
		}
	}

	void VisualComponent::onRendererTextureChanged()
	{
		if (m_visual)
		{
			m_visual->setRendererTexturesChanged();
		}
		if (m_shadowVisual)
		{
			m_shadowVisual->setRendererTexturesChanged();
		}
	}

	void VisualComponent::onRendererSamplerStateChanged()
	{
		if (m_visual)
		{
			m_visual->setRendererSamplerStatesChanged();
		}
		if (m_shadowVisual)
		{
			m_shadowVisual->setRendererSamplerStatesChanged();
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<VisualComponent>("VisualComponent")
			.constructor<>()
			.property("enableShadow", &VisualComponent::get_enableShadow, &VisualComponent::set_enableShadow)
			.property("renderer", &VisualComponent::get_renderer, &VisualComponent::set_renderer)
			.property("rendererCategory", &VisualComponent::get_rendererCategory, &VisualComponent::set_rendererCategory)
			.property("rasterizerDesc", &VisualComponent::get_rasterizerDesc, &VisualComponent::set_rasterizerDesc)
			.property("depthStencilDesc", &VisualComponent::get_depthStencilDesc, &VisualComponent::set_depthStencilDesc)
			.property("blendDesc", &VisualComponent::get_blendDesc, &VisualComponent::set_blendDesc)
			.property("material", &VisualComponent::get_material, &VisualComponent::set_material);
	}
}