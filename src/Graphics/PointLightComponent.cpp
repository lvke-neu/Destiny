#include "PointLightComponent.h"
#include "VisualComponent.h"
#include "Graphics/VisualScene.h"

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
		traversalPointLightChanged(m_scene, pointLights);
    }

	void PointLightComponent::onLeaveScene()
	{
		std::vector<PointLight> pointLights;
		traversal(m_scene, pointLights, true);
		traversalPointLightChanged(m_scene, pointLights);
	}

    void PointLightComponent::onNodeTransformChanged()
    {
		if (m_enable)
		{
			std::vector<PointLight> pointLights;
			traversal(m_scene, pointLights);
			traversalPointLightChanged(m_scene, pointLights);
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
		traversalPointLightChanged(m_scene, pointLights);
	}

	void PointLightComponent::set_intensity(float intensity)
	{
		m_intensity = intensity;
		std::vector<PointLight> pointLights;
		traversal(m_scene, pointLights);
		traversalPointLightChanged(m_scene, pointLights);
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
				pointLights.push_back({ {plComponent->get_color()}, {plComponent->get_node()->get_transform().get_translation()}, plComponent->get_intensity() });
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			traversal(childNode, pointLights, ignoreSelf);
		}
	}

	void PointLightComponent::traversalPointLightChanged(std::shared_ptr<Node> node, const std::vector<PointLight>& pointLights)
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
					visualComponent->onPointLightChanged(pointLights);
				}
			}
		}

		for (const auto& childNode : node->getChilds())
		{
			traversalPointLightChanged(childNode, pointLights);
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