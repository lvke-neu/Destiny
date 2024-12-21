#include "ModelComponent.h"
#include "Model.h"

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
			m_model->load();
			m_modelChanged = true;
		}
	}

	void ModelComponent::onUpdate(float deltaTime)
	{
		if (m_modelChanged && m_model->isLoadingSucceed())
		{
			if (m_node)
			{
				//TODO: add to node
				m_modelChanged = false;
			}
		}
	}
}