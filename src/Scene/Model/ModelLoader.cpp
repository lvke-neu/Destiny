#include "ModelLoader.h"
#include "Model.h"

namespace Destiny
{
	ModelLoader::ModelLoader()
	{

	}

	ModelLoader::~ModelLoader()
	{

	}

	void ModelLoader::loadAsset(std::shared_ptr<Asset> asset)
	{
		_sleep(3000);
		asset->loadSucceeded__();
	}
}