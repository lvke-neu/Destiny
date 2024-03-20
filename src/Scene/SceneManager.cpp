#include "SceneManager.h"
#include "Engine/Engine.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoaderManager.h"

namespace Destiny
{
	SceneManager::SceneManager()
	{
		
	}

	SceneManager::~SceneManager()
	{

	}

	void SceneManager::initialize()
	{
		const char* path = "builtin://1.txt";
		auto blobloader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		if (blobloader)
		{
			auto blobholder = blobloader->createBlobHolder(path);
			if (blobholder)
			{
				blobholder->load(1);
			}
		}
	}

	void SceneManager::uninitialize()
	{

	}

	void SceneManager::update()
	{
		
	}
}