#include "Model.h"
#include "ModelLoader.h"
#include "Engine/BlobHolder.h"
#include "Engine/Engine.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobLoaderManager.h"
#include "Graphics/VisualComponent.h"
#include "Graphics/SamplerState.h"
#include "Graphics/Texture.h"
#include "Graphics/Mesh.h"

namespace Destiny
{
	std::shared_ptr<ModelLoader> Model::s_modelLoader = std::make_shared<ModelLoader>();
	Model::Model() :
		m_node(nullptr),
		m_mergedAABB{ { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } }
	{

	}

	Model::~Model()
	{

	}

	std::shared_ptr<Model> Model::Create(const char* path)
	{
		auto blobLoader = Destiny::Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		if (blobLoader)
		{
			auto blobHolder = blobLoader->createBlobHolder(path);

			if (blobHolder)
			{
				std::shared_ptr<Model> model = std::make_shared<Model>();
				model->initialize(s_modelLoader, blobHolder);

				return model;
			}
		}

		return nullptr;
	}

	void Model::onDataLoaded()
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent && visualComponent->getVisual())
			{
				visualComponent->getVisual()->load(0);

				if (visualComponent->getVisual()->getMesh())
				{
					visualComponent->getVisual()->getMesh()->setBoundingBox(m_mergedAABB);
				}
			}	
		}
	}

	void Model::setEnable(bool enable)
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent)
			{
				visualComponent->set_enable(enable);
			}
		}
	}

	void Model::setEnableShadow(bool enableShadow)
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent)
			{
				visualComponent->set_enableShadow(enableShadow);
			}
		}
	}

	void Model::setRenderer(std::string renderer)
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent)
			{
				visualComponent->set_renderer(renderer);
			}
		}
	}

	void Model::setShadowRenderer(std::string renderer)
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent)
			{
				visualComponent->set_shadowRenderer(renderer);
			}
		}
	}

	void Model::setRendererCategory(RendererCategory rendererCategory)
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent)
			{
				visualComponent->set_rendererCategory(rendererCategory);
			}
		}
	}

	void Model::setRasterizerDesc(D3D11_RASTERIZER_DESC desc)
	{
		for (const auto& visualComponent : m_visualComponents)
		{
			if (visualComponent)
			{
				visualComponent->set_rasterizerDesc(desc);
			}
		}
	}
}