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
		m_modelChanged(false),
		m_renderer("builtin://renderer/forward_pbr.hlsl"),
		m_rendererCategory(RendererCategory::ForwardOpaque),\
		m_rasterizerDesc(CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT()))
	{

	}

	ModelComponent::~ModelComponent()
	{
		if (m_model && m_model->getNode())
		{
			m_model->getNode()->removeFromParent();
		}
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

	void ModelComponent::set_renderer(std::string renderer)
	{
		if (m_renderer != renderer)
		{
			m_renderer = renderer;
			if (m_model)
			{
				m_model->setRenderer(m_renderer);
			}
		}
	}

	void ModelComponent::set_rendererCategory(RendererCategory rendererCategory)
	{
		if (m_rendererCategory != rendererCategory)
		{
			m_rendererCategory = rendererCategory;
			if (m_model)
			{
				m_model->setRendererCategory(m_rendererCategory);
			}
		}
	}

	void ModelComponent::set_rasterizerDesc(D3D11_RASTERIZER_DESC rasterizerDesc)
	{
		m_rasterizerDesc = rasterizerDesc;
		if (m_model)
		{
			m_model->setRasterizerDesc(rasterizerDesc);
		}
	}

	void ModelComponent::onUpdate(float deltaTime)
	{
		if (m_modelChanged && m_model && m_model->isLoadingSucceed() && m_model->getNode())
		{
			m_model->onDataLoaded();
			m_model->getNode()->addToParent(m_node);
			m_model->setEnable(m_enable);
			m_model->setRenderer(m_renderer);
			m_model->setRendererCategory(m_rendererCategory);
			//m_model.reset();
			m_modelChanged = false;
			LOG_INFO("Model:{0} load successfully", m_path);	
		}
	}

	void ModelComponent::onPropertyChanged(const std::string& property)
	{
		if (property == "enable")
		{
			if (m_model)
			{
				m_model->setEnable(m_enable);
			}
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ModelComponent>("ModelComponent")
			.constructor<>()
			.property("path", &ModelComponent::get_path, &ModelComponent::set_path)
			.property("renderer", &ModelComponent::get_renderer, &ModelComponent::set_renderer)
			.property("rendererCategory", &ModelComponent::get_rendererCategory, &ModelComponent::set_rendererCategory)
			.property("rasterizerDesc", &ModelComponent::get_rasterizerDesc, &ModelComponent::set_rasterizerDesc);
	}
}