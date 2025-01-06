#include "ModelComponent.h"
#include "Model.h"
#include "Engine/Node.h"
#include "Engine/Utility.h"
#include "Graphics/VisualComponent.h"
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

	void ModelComponent::set_path(std::string path)
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
			m_model->loadVisuals();
			m_model->getNode()->addToParent(m_node);
			m_model->setEnable(m_enable);
			m_model.reset();
			m_modelChanged = false;
			LOG_INFO("Model:{0} load successfully", m_path);	
		}
	}

	void ModelComponent::onPropertyChanged(const std::string& property)
	{
		if (property == "enable")
		{
			std::queue<std::shared_ptr<Node>> nodes;
			nodes.push(m_node);
			while (!nodes.empty())
			{
				auto topNode = nodes.front();
				nodes.pop();
				if (topNode)
				{
					for (const auto& component : topNode->getComponents())
					{
						if (std::dynamic_pointer_cast<VisualComponent>(component))
						{
							component->set_enable(m_enable);
						}
					}
					for (const auto& node : topNode->getChilds())
					{
						nodes.push(node);
					}
				}
			}
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ModelComponent>("ModelComponent")
			.constructor<>()
			.property("path", &ModelComponent::get_path, &ModelComponent::set_path);
	}
}