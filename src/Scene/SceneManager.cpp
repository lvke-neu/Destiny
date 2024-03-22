#include "SceneManager.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoaderManager.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderStates.h"
#include <d3d11.h>

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

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(12, 0, data);
		vertexBuffer->load(0);

		std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>("builtin://renderer/basic.rdr");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>(std::shared_ptr<CD3D11_RASTERIZER_DESC>(new CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT())), std::shared_ptr<CD3D11_DEPTH_STENCIL_DESC>(new CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT())), std::shared_ptr<CD3D11_BLEND_DESC>(new CD3D11_BLEND_DESC(CD3D11_DEFAULT())));
		renderStates->load(0);
		
		int i = 0;
		i++;
	}

	void SceneManager::uninitialize()
	{

	}

	void SceneManager::update()
	{
		
	}
}