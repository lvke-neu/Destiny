#include "DirectionLightComponent.h"
#include "VisualComponent.h"
#include "Graphics/VisualScene.h"
#include "RenderSystem.h"
#include "DeferredOpaquePipeline.h"

namespace Destiny
{
	DirectionLightComponent::DirectionLightComponent() :
		m_color({ 1.0f, 1.0f, 1.0f, 1.0f }),
		m_intensity(1.0f)
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
		setDirectionLightRendererConstant(directionLights);
    }

	void DirectionLightComponent::onLeaveScene()
	{
		std::vector<DirectionLight> directionLights;
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene);
		setDirectionLightRendererConstant(directionLights);
	}

    void DirectionLightComponent::onNodeTransformChanged()
    {
		std::vector<DirectionLight> directionLights;
		traversal(m_scene, directionLights);
		auto renderSystem = std::static_pointer_cast<RenderSystem>(Engine::GetInstance()->getGraphicsSystem());
		auto deferredOpaquePipeline = std::static_pointer_cast<DeferredOpaquePipeline>(renderSystem->getDeferredOpaquePipeline());
		deferredOpaquePipeline->onRendererConstantChanged();
		notifyVisualRendererConstantChanged(m_scene);
		setDirectionLightRendererConstant(directionLights);
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
		setDirectionLightRendererConstant(directionLights);
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
		setDirectionLightRendererConstant(directionLights);
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

	void DirectionLightComponent::setDirectionLightRendererConstant(const std::vector<DirectionLight>& directionLights)
	{
		std::shared_ptr<Blob> blob = std::make_shared<Blob>(directionLights.size() * sizeof(DirectionLight));
		blob->copyfrom((void*)directionLights.data(), blob->getLength());

		for (const auto& renderer : Renderer::s_cache)
		{
			renderer.second->setConstant("g_directionLightCount", (int)directionLights.size());
			renderer.second->setConstant("g_directionLights", blob);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<DirectionLightComponent>("DirectionLightComponent")
			.constructor<>()
			.property("color", &DirectionLightComponent::get_color, &DirectionLightComponent::set_color)
			.property("intensity", &DirectionLightComponent::get_intensity, &DirectionLightComponent::set_intensity);
	}
}