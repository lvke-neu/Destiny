#include "ModelComponent.h"
#include "Model.h"
#include "Engine/Node.h"
#include "Engine/Utility.h"
#include "Graphics/VisualComponent.h"
#include "../RuntimeEffect/BoxVisualizationComponent.h"
#include <queue>

namespace Destiny
{
	ModelComponent::ModelComponent() :
		m_model(nullptr),
		m_modelChanged(false),
		m_renderer("builtin://renderer/forward_pbr_model.hlsl"),
		m_shadowRenderer("builtin://renderer/forward_pbr_model_shadow.hlsl"),
		m_rendererCategory(RendererCategory::ForwardOpaque),
		m_rasterizerDesc(CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT())),
		m_enableShadow(true),
		m_animation(""),
		m_animationIndex(-1)
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

	void ModelComponent::set_shadowRenderer(std::string shadowRenderer)
	{
		if (m_shadowRenderer != shadowRenderer)
		{
			m_shadowRenderer = shadowRenderer;
			if (m_model)
			{
				m_model->setShadowRenderer(m_shadowRenderer);
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
			m_model->setRasterizerDesc(m_rasterizerDesc);
		}
	}

	void ModelComponent::set_enableShadow(bool enableShadow)
	{
		m_enableShadow = enableShadow;
		if (m_model)
		{
			m_model->setEnableShadow(m_enableShadow);
		}
	}

	void ModelComponent::onUpdate(float deltaTime)
	{
		if (m_modelChanged && m_model && m_model->isLoadingSucceed() && m_model->getNode())
		{
			m_model->onDataLoaded();
			m_model->getNode()->addToParent(m_node);
			m_model->setEnable(m_enable);
			m_model->setEnableShadow(m_enableShadow);
			m_model->setRenderer(m_renderer);
			m_model->setShadowRenderer(m_shadowRenderer);
			m_model->setRendererCategory(m_rendererCategory);
			//m_model.reset();
			m_modelChanged = false;


			//auto boxVisualizationComponent = std::make_shared<BoxVisualizationComponent>();
			//boxVisualizationComponent->modifyMesh(m_model->getBox());

			//auto boxVisualizationNode = std::make_shared<Node>("AABB Visualization");
			//boxVisualizationNode->set_serializable(false);
			//boxVisualizationNode->addComponent(boxVisualizationComponent);
			//boxVisualizationNode->addToParent(m_node);

			LOG_INFO("Model:{0} load successfully", m_path);	
		}

		if (m_model && m_model->isLoadingSucceed())
		{
			m_model->updateAnimation(deltaTime);
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


	std::string ModelComponent::get_animation()
	{
		if (m_model)
		{
			return m_model->get_animation();
		}
		return m_animation;
	}

	void ModelComponent::set_animation(std::string animation)
	{
		m_animation = animation;
		if (m_model)
		{
			return m_model->set_animation(m_animation);
		}
	}

	unsigned int ModelComponent::get_animationIndex()
	{
		if (m_model)
		{
			return m_model->get_animationIndex();
		}
		return m_animationIndex;
	}

	void ModelComponent::set_animationIndex(unsigned int animationIndex)
	{
		m_animationIndex = animationIndex;
		if (m_model)
		{
			m_model->set_animationIndex(m_animationIndex);
		}
	}

	unsigned int ModelComponent::get_animationCount()
	{
		if (m_model)
		{
			return m_model->get_animationCount();
		}

		return 0;
	}

	void ModelComponent::set_animationCount(unsigned int animationCount)
	{
		if (m_model)
		{
			return m_model->set_animationCount(animationCount);
		}
	}

	float ModelComponent::get_animationSpeed()
	{
		if (m_model)
		{
			return m_model->get_animationSpeed();
		}
		return 1.0f;
	}

	void ModelComponent::set_animationSpeed(float animationSpeed)
	{
		if (m_model)
		{
			return m_model->set_animationSpeed(animationSpeed);
		}
	}

	RTTR_REGISTRATION
	{
		rttr::registration::class_<ModelComponent>("ModelComponent")
			.constructor<>()
			.property("enableShadow", &ModelComponent::get_enableShadow, &ModelComponent::set_enableShadow)
			.property("path", &ModelComponent::get_path, &ModelComponent::set_path)
			.property("renderer", &ModelComponent::get_renderer, &ModelComponent::set_renderer)
			.property("shadowRenderer", &ModelComponent::get_shadowRenderer, &ModelComponent::set_shadowRenderer)
			.property("rendererCategory", &ModelComponent::get_rendererCategory, &ModelComponent::set_rendererCategory)
			.property("rasterizerDesc", &ModelComponent::get_rasterizerDesc, &ModelComponent::set_rasterizerDesc)
			.property("animation", &ModelComponent::get_animation, &ModelComponent::set_animation)
			.property("animationIndex", &ModelComponent::get_animationIndex, &ModelComponent::set_animationIndex)
			.property("animationCount", &ModelComponent::get_animationCount, &ModelComponent::set_animationCount)
			.property("animationSpeed", &ModelComponent::get_animationSpeed, &ModelComponent::set_animationSpeed);
	}
}