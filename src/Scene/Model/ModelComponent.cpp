#include "ModelComponent.h"
#include "Model.h"
#include "Graphics/VisualComponent.h"
#include "Graphics/RenderPass.h"
#include "Graphics/Mesh.h"
#include "Engine/Node.h"
#include "Engine/Utility.h"
#include <queue>

namespace Destiny
{
	ModelComponent::ModelComponent() :
		m_model(nullptr),
		m_modelChanged(false)
	{

	}

	ModelComponent::~ModelComponent()
	{

	}

	void ModelComponent::set_path(const std::string& path)
	{
		if (m_path != path)
		{
			m_path = path;
			m_model.reset();
			m_model = Model::Create(path.c_str());
			if (m_model)
			{
				m_model->load();
				m_modelChanged = true;
			}
		}
	}

	void ModelComponent::onUpdate(float deltaTime)
	{
		if (m_modelChanged && m_model && m_model->isLoadingSucceed() && m_model->getNode())
		{
			auto modelNode = m_model->getNode();
			if (modelNode)
			{
				std::queue<std::shared_ptr<Node>> nodes;
				nodes.push(modelNode);
				while (!nodes.empty())
				{
					auto topNode = nodes.front();
					nodes.pop();
					if (topNode)
					{
						for (const auto& component : topNode->getComponents())
						{
							auto visualComponent = std::dynamic_pointer_cast<VisualComponent>(component);
							if (visualComponent && visualComponent->getVisual())
							{
								if (visualComponent->getVisual()->getRenderPass())
								{
									visualComponent->getVisual()->getRenderPass()->load(0);
								}
								if (visualComponent->getVisual()->getMesh())
								{
									visualComponent->getVisual()->getMesh()->load(0);
								}
							}
						}
						for (const auto& node : topNode->getChilds())
						{
							nodes.push(node);
						}
					}
				}


				modelNode->addToParent(m_node);
				LOG_INFO("Model:{0} load successfully", m_path);
			}

			m_modelChanged = false;
		}
	}
}