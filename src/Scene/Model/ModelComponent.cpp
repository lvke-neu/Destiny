#include "ModelComponent.h"
#include "Model.h"
#include "Engine/Node.h"

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
				modelNode->addToParent(m_node);
			}
			m_modelChanged = false;
		}
	}
}