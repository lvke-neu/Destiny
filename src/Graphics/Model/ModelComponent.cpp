#include "ModelComponent.h"
#include "Model.h"
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
			m_model.reset();
			m_modelChanged = false;
			LOG_INFO("Model:{0} load successfully", m_path);	
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ModelComponent>("ModelComponent")
			.constructor<>()
			.property("path", &ModelComponent::get_path, &ModelComponent::set_path);
	}
}