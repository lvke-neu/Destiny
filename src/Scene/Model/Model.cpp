#include "Model.h"
#include "ModelLoader.h"
#include "Engine/BlobHolder.h"
#include "Engine/Engine.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobLoaderManager.h"

namespace Destiny
{
	std::shared_ptr<ModelLoader> Model::s_modelLoader = std::make_shared<ModelLoader>();
	Model::Model() :
		m_node(nullptr)
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
}