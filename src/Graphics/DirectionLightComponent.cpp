#include "DirectionLightComponent.h"
#include "VisualComponent.h"
#include "Graphics/VisualScene.h"

namespace Destiny
{
	DirectionLightComponent::DirectionLightComponent() :
		m_color({ 1.0f, 1.0f, 1.0f, 1.0f }),
		m_intensity(1.0f)
	{

	}

	void DirectionLightComponent::onEnterScene()
    {
		traversalLightDirectionChanged(m_scene);
		traversalLightColorChanged(m_scene);
		traversalLightIntensityChanged(m_scene);
    }

    void DirectionLightComponent::onNodeTransformChanged()
    {
		traversalLightDirectionChanged(m_scene);
    }

	void DirectionLightComponent::set_color(Color32 color)
	{
		m_color = color;
		traversalLightColorChanged(m_scene);
	}

	void DirectionLightComponent::set_intensity(float intensity)
	{
		m_intensity = intensity;
		traversalLightIntensityChanged(m_scene);
	}

	void DirectionLightComponent::traversalLightDirectionChanged(std::shared_ptr<Node> node)
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
					visualComponent->onLightDirectionChanged(m_node->get_transform().get_rotation());
				}
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			traversalLightDirectionChanged(childNode);
		}
    }

	void DirectionLightComponent::traversalLightColorChanged(std::shared_ptr<Node> node)
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
					visualComponent->onLightColorChanged(m_color);
				}
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			traversalLightColorChanged(childNode);
		}
	}

	void DirectionLightComponent::traversalLightIntensityChanged(std::shared_ptr<Node> node)
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
					visualComponent->onLightIntensityChanged(m_intensity);
				}
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			traversalLightIntensityChanged(childNode);
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