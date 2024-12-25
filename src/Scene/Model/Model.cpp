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
						if (material->t_ambient)
						{
							material->t_ambient->load();
						}
						if (material->t_diffuse)
						{
							material->t_diffuse->load();
						}
						if (material->t_specular)
						{
							material->t_specular->load();
						}
						visual->setConstant("c_has_c_ambient", material->c_has_c_ambient);
						visual->setConstant("c_has_c_diffuse", material->c_has_c_diffuse);
						visual->setConstant("c_has_c_specular", material->c_has_c_specular);
						visual->setConstant("c_has_t_ambient", material->c_has_t_ambient);
						visual->setConstant("c_has_t_diffuse", material->c_has_t_diffuse);
						visual->setConstant("c_has_t_specular", material->c_has_t_specular);
						visual->setConstant("c_ambient", material->c_ambient);
						visual->setConstant("c_diffuse", material->c_diffuse);
						visual->setConstant("c_specular", material->c_specular);
						
						visual->setShaderResource("t_ambient", material->t_ambient);
						visual->setShaderResource("t_diffuse", material->t_diffuse);
						visual->setShaderResource("t_specular", material->t_specular);
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
}