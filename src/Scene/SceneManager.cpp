#include "SceneManager.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoaderManager.h"
#include "Graphics/IndexBuffer.h"

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
		//const char* path = "builtin://1.txt";
		//auto blobloader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		//if (blobloader)
		//{
		//	auto blobholder = blobloader->createBlobHolder(path);
		//	if (blobholder)
		//	{
		//		blobholder->load(1);
		//	}
		//}
		
		std::shared_ptr<Blob> data = std::make_shared<Blob>(250);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer::IndexType::Index16, data);
		indexBuffer->load(0);
	}

	void SceneManager::uninitialize()
	{

	}

	void SceneManager::update()
	{
		
	}
}