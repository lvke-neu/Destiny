#include "Model.h"
#include "ModelLoader.h"
#include "Material.h"
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

	void Model::loadVisuals()
	{
		for (const auto& visual_material_pair : m_visuals)
		{
			if (visual_material_pair.first )
			{
				auto visual = visual_material_pair.first->getVisual();
				if (visual)
				{
					visual->load(0);

					auto material = visual_material_pair.second;
					if (material)
					{
						if (material->s_sampler)
						{
							material->s_sampler->load();
						}
						if (material->t_albedo)
						{
							material->t_albedo->load();
						}
						if (material->t_normal)
						{
							material->t_normal->load();
						}
						if (material->t_metallic)
						{
							material->t_metallic->load();
						}
						if (material->t_roughness)
						{
							material->t_roughness->load();
						}
						if (material->t_ao)
						{
							material->t_ao->load();
						}

						visual->setShaderResource("t_albedo", material->t_albedo);
						visual->setShaderResource("t_normal", material->t_normal);
						visual->setShaderResource("t_metallic", material->t_metallic);
						visual->setShaderResource("t_roughness", material->t_roughness);
						visual->setShaderResource("t_ao", material->t_ao);
						visual->setSamplerSate("s_sampler", material->s_sampler);
					}

					if (visual->getMesh())
					{
						visual->getMesh()->setBoundingBox(m_mergedAABB);
					}
				}	
			}
		}
	}

	void Model::setEnable(bool enable)
	{
		for (const auto& visual_material_pair : m_visuals)
		{
			if (visual_material_pair.first)
			{
				visual_material_pair.first->set_enable(enable);
			}
		}
	}
}