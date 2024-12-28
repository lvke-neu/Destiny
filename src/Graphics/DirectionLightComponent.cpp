#include "DirectionLightComponent.h"
#include "VisualComponent.h"
#include "Graphics/VisualScene.h"

namespace Destiny
{
    void DirectionLightComponent::onEnterScene()
    {
		traversalLightDirectionChanged(m_scene);
    }

    void DirectionLightComponent::onNodeTransformChanged()
    {
		traversalLightDirectionChanged(m_scene);
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

}