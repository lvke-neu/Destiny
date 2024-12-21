#include "Model.h"
#include "ModelLoader.h"
#include "Engine/BlobHolder.h"

namespace Destiny
{
	std::shared_ptr<ModelLoader> Model::s_modelLoader = std::make_shared<ModelLoader>();
	Model::Model()
	{

	}

	Model::~Model()
	{

	}

	std::shared_ptr<Model> Model::Create(const char* path)
	{
		std::shared_ptr<Model> model = std::make_shared<Model>();
		std::shared_ptr<BlobHolder> blobHolder = std::make_shared<BlobHolder>();
		
		blobHolder->setPath(path);
		model->initialize(s_modelLoader, blobHolder);

		return model;
	}
}