#include "DirectionLightComponent.h"
#include "VisualComponent.h"
#include "Graphics/VisualScene.h"
#include "RenderSystem.h"
#include "DeferredOpaquePipeline.h"
#include "ShadowMapPipeline.h"
#include "Math/Math.h"

namespace Destiny
{
	DirectionLightComponent::DirectionLightComponent() :
		m_color({ 1.0f, 1.0f, 1.0f, 1.0f }),
		m_intensity(1.0f),
		m_viewPortWidth(50.0f),
		m_viewPortHeight(50.0f),
		m_nearz(0.1f),
		m_farz(1000.0f),
		m_lightDiscoefficient(-400.0f),
		m_resolutionWidth(2048.0f),
		m_resolutionHeight(2048.0f),
		m_shadowBias(0.0f)
	{

	}

	void DirectionLightComponent::onEnterScene()
    {
		if (!m_enable)
		{
			return;
		}

		std::vector<DirectionLight> directionLights;
		traversal(m_scene, directionLights);
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene);
		updateDirectionLightRendererConstant(directionLights);
		updateShadowMapRendererConstant();
		notifyShadoMapPipiline();
    }

	void DirectionLightComponent::onLeaveScene()
	{
		std::vector<DirectionLight> directionLights;
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene);
		updateDirectionLightRendererConstant(directionLights);
	}

    void DirectionLightComponent::onNodeTransformChanged()
    {
		std::vector<DirectionLight> directionLights;
		traversal(m_scene, directionLights);
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene);
		updateDirectionLightRendererConstant(directionLights);
		updateShadowMapRendererConstant();
		notifyShadoMapPipiline();
    }

	void DirectionLightComponent::onPropertyChanged(const std::string& property)
	{
		if (property == "enable")
		{
			m_enable ? onEnterScene() : onLeaveScene();
		}
	}

	void DirectionLightComponent::set_color(Color color)
	{
		m_color = color;
		std::vector<DirectionLight> directionLights;
		traversal(m_scene, directionLights);
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene);
		updateDirectionLightRendererConstant(directionLights);
	}

	void DirectionLightComponent::set_intensity(float intensity)
	{
		m_intensity = intensity;
		std::vector<DirectionLight> directionLights;
		traversal(m_scene, directionLights);
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene);
		updateDirectionLightRendererConstant(directionLights);
	}

	void DirectionLightComponent::set_viewPortWidth(float viewPortWidth)
	{
		if (viewPortWidth <= 0)
		{
			return;
		}
		m_viewPortWidth = viewPortWidth;
		notifyVisualRendererConstantChanged(m_scene);
		updateShadowMapRendererConstant();
		notifyShadoMapPipiline();
	}

	void DirectionLightComponent::set_viewPortHeight(float viewPortHeight)
	{
		if (viewPortHeight <= 0)
		{
			return;
		}
		m_viewPortHeight = viewPortHeight;
		notifyVisualRendererConstantChanged(m_scene);
		updateShadowMapRendererConstant();
		notifyShadoMapPipiline();
	}

	void DirectionLightComponent::set_nearz(float nearz)
	{
		m_nearz = nearz;
		notifyVisualRendererConstantChanged(m_scene);
		updateShadowMapRendererConstant();
		notifyShadoMapPipiline();
	}

	void DirectionLightComponent::set_farz(float farz)
	{
		m_farz = farz;
		notifyVisualRendererConstantChanged(m_scene);
		updateShadowMapRendererConstant();
		notifyShadoMapPipiline();
	}

	void DirectionLightComponent::set_lightDiscoefficient(float lightDiscoefficient)
	{
		m_lightDiscoefficient = lightDiscoefficient;
		notifyVisualRendererConstantChanged(m_scene);
		updateShadowMapRendererConstant();
		notifyShadoMapPipiline();
	}

	void DirectionLightComponent::set_resolutionWidth(float resolutionWidth)
	{
		m_resolutionWidth = resolutionWidth;
		notifyVisualRendererConstantChanged(m_scene);
		updateShadowMapRendererConstant();
		notifyShadoMapPipiline();
	}

	void DirectionLightComponent::set_resolutionHeight(float resolutionHeight)
	{
		m_resolutionHeight = resolutionHeight;
		notifyVisualRendererConstantChanged(m_scene);
		updateShadowMapRendererConstant();
		notifyShadoMapPipiline();
	}

	void DirectionLightComponent::set_shadowBias(float shadowBias)
	{
		m_shadowBias = shadowBias;
		notifyVisualRendererConstantChanged(m_scene);
		updateShadowMapRendererConstant();
		notifyShadoMapPipiline();
	}

	void DirectionLightComponent::traversal(std::shared_ptr<Node> node, std::vector<DirectionLight>& directionLights, bool ignoreSelf)
	{
		if (!node)
		{
			return;
		}

		for (const auto& component : node->getComponents())
		{
			auto dlComponent = std::dynamic_pointer_cast<DirectionLightComponent>(component);
			if (ignoreSelf && dlComponent == shared_from_this())
			{
				continue;
			}
			if (dlComponent && dlComponent->get_enable() && dlComponent->get_node())
			{
				directionLights.push_back({ {dlComponent->get_color()}, {dlComponent->get_node()->get_rotation()}, dlComponent->get_intensity() });
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			traversal(childNode, directionLights, ignoreSelf);
		}
	}

	void DirectionLightComponent::notifyVisualRendererConstantChanged(std::shared_ptr<Node> node)
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
					visualComponent->onRendererConstantChanged();
				}
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			notifyVisualRendererConstantChanged(childNode);
		}
	}

	void DirectionLightComponent::notifyVisualRendererTextureChanged(std::shared_ptr<Node> node)
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
					visualComponent->onRendererTextureChanged();
				}
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			notifyVisualRendererTextureChanged(childNode);
		}
	}

	void DirectionLightComponent::notifyVisualRendererSamplerStateChanged(std::shared_ptr<Node> node)
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
					visualComponent->onRendererSamplerStateChanged();
				}
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			notifyVisualRendererSamplerStateChanged(childNode);
		}
	}

	void DirectionLightComponent::updateDirectionLightRendererConstant(const std::vector<DirectionLight>& directionLights)
	{
		std::shared_ptr<Blob> blob = std::make_shared<Blob>(directionLights.size() * sizeof(DirectionLight));
		blob->copyfrom((void*)directionLights.data(), blob->getLength());


		for (const auto& renderer : Renderer::s_cache)
		{
			renderer.second->setConstant("g_directionLightCount", (int)directionLights.size());
			renderer.second->setConstant("g_directionLights", blob);
		}
	}

	void DirectionLightComponent::updateShadowMapRendererConstant()
	{
		if (!m_node)
		{
			return;
		}

		using namespace DirectX;

		XMFLOAT3 rotation = m_node->get_rotation();

		XMVECTOR normalizedLightDir = XMLoadFloat3(&rotation);
		normalizedLightDir = XMVector3Normalize(normalizedLightDir);

		XMMATRIX shadowView = XMMatrixLookAtLH(normalizedLightDir * m_lightDiscoefficient, g_XMZero, g_XMIdentityR1);

		DirectX::XMMATRIX T
		(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f
		);


		for (const auto& renderer : Renderer::s_cache)
		{
			renderer.second->setConstant("g_shadowView", XMMatrixTranspose(shadowView));
			renderer.second->setConstant("g_shadowProj", XMMatrixTranspose(XMMatrixOrthographicLH((float)m_viewPortWidth, (float)m_viewPortHeight, m_nearz, m_farz)));
			renderer.second->setConstant("T", XMMatrixTranspose(T));
			renderer.second->setConstant("g_shadowBias", m_shadowBias);
			renderer.second->setConstant("g_dx", 1.0f / m_resolutionWidth);
			renderer.second->setConstant("g_dy", 1.0f / m_resolutionHeight);
		}
	}

	void DirectionLightComponent::notifyShadoMapPipiline()
	{
		auto renderSystem =std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto shadowMapPipeline = std::static_pointer_cast<ShadowMapPipeline>(renderSystem->getShadowMapPipeline());
		shadowMapPipeline->onResize(m_resolutionWidth, m_resolutionHeight);

		notifyVisualRendererTextureChanged(m_scene);
		notifyVisualRendererSamplerStateChanged(m_scene);
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<DirectionLightComponent>("DirectionLightComponent")
			.constructor<>()
			.property("color", &DirectionLightComponent::get_color, &DirectionLightComponent::set_color)
			.property("intensity", &DirectionLightComponent::get_intensity, &DirectionLightComponent::set_intensity)
			.property("shadow_viewPortWidth", &DirectionLightComponent::get_viewPortWidth, &DirectionLightComponent::set_viewPortWidth)
			.property("shadow_viewPortHeight", &DirectionLightComponent::get_viewPortHeight, &DirectionLightComponent::set_viewPortHeight)
			.property("shadow_nearz", &DirectionLightComponent::get_nearz, &DirectionLightComponent::set_nearz)
			.property("shadow_farz", &DirectionLightComponent::get_farz, &DirectionLightComponent::set_farz)
			.property("shadow_lightDiscoefficient", &DirectionLightComponent::get_lightDiscoefficient, &DirectionLightComponent::set_lightDiscoefficient)
			.property("shadow_resolutionWidth", &DirectionLightComponent::get_resolutionWidth, &DirectionLightComponent::set_resolutionWidth)
			.property("shadow_resolutionHeight", &DirectionLightComponent::get_resolutionHeight, &DirectionLightComponent::set_resolutionHeight)
			.property("shadow_bias", &DirectionLightComponent::get_shadowBias, &DirectionLightComponent::set_shadowBias);
	}
}