#include "PointLightComponent.h"
#include "VisualComponent.h"
#include "Graphics/VisualScene.h"
#include "RenderSystem.h"
#include "DeferredOpaquePipeline.h"

namespace Destiny
{
	PointLightComponent::PointLightComponent() :
		m_color({ 1.0f, 1.0f, 1.0f, 1.0f }),
		m_intensity(1.0f)
	{

	}

	void PointLightComponent::onEnterScene()
    {
		std::vector<PointLight> pointLights;
		traversal(m_scene, pointLights);
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene);
		setPointLightRendererConstant(pointLights);
    }

	void PointLightComponent::onLeaveScene()
	{
		std::vector<PointLight> pointLights;
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene);
		setPointLightRendererConstant(pointLights);
	}

    void PointLightComponent::onNodeTransformChanged()
    {
		if (m_enable)
		{
			std::vector<PointLight> pointLights;
			traversal(m_scene, pointLights);
			auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
			auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
			deferredOpaquePipeline->onRendererConstantChanged();
			notifyVisualRendererConstantChanged(m_scene);
			setPointLightRendererConstant(pointLights);
		}
    }

	void PointLightComponent::onPropertyChanged(const std::string& property)
	{
		if (property == "enable")
		{
			m_enable ? onEnterScene() : onLeaveScene();
		}
	}

	void PointLightComponent::set_color(Color color)
	{
		m_color = color;
		std::vector<PointLight> pointLights;
		traversal(m_scene, pointLights);
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene);
		setPointLightRendererConstant(pointLights);
	}

	void PointLightComponent::set_intensity(float intensity)
	{
		m_intensity = intensity;
		std::vector<PointLight> pointLights;
		traversal(m_scene, pointLights);
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene);
		setPointLightRendererConstant(pointLights);
	}

	void PointLightComponent::traversal(std::shared_ptr<Node> node, std::vector<PointLight>& pointLights, bool ignoreSelf)
	{
		if (!node)
		{
			return;
		}

		for (const auto& component : node->getComponents())
		{
			auto plComponent = std::dynamic_pointer_cast<PointLightComponent>(component);
			if (ignoreSelf && plComponent == shared_from_this())
			{
				continue;
			}
			if (plComponent && plComponent->get_enable() && plComponent->get_node())
			{
				pointLights.push_back({ {plComponent->get_color()}, {plComponent->get_node()->get_translation()}, plComponent->get_intensity() });
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			traversal(childNode, pointLights, ignoreSelf);
		}
	}

	void PointLightComponent::notifyVisualRendererConstantChanged(std::shared_ptr<Node> node)
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

	void PointLightComponent::setPointLightRendererConstant(const std::vector<PointLight>& pointLights)
	{
		std::shared_ptr<Blob> blob = std::make_shared<Blob>(pointLights.size() * sizeof(PointLight));
		blob->copyfrom((void*)pointLights.data(), blob->getLength());

		for (const auto& renderer : Renderer::s_cache)
		{
			renderer.second->setConstant("g_pointLightCount", (int)pointLights.size());
			renderer.second->setConstant("g_pointLights", blob);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<PointLightComponent>("PointLightComponent")
			.constructor<>()
			.property("color", &PointLightComponent::get_color, &PointLightComponent::set_color)
			.property("intensity", &PointLightComponent::get_intensity, &PointLightComponent::set_intensity);
	}
}