#include "Texture.h"
#include "TextureLoader.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoader.h"
#include "Engine/Utility.h"
#include <d3d11.h>

namespace Destiny
{
	std::shared_ptr<TextureLoader> Texture::s_textureLoader = std::make_shared<TextureLoader>();

	Texture::Texture() :
		m_resource(nullptr),
		m_shaderResourceView(nullptr)
	{

	}

	Texture::~Texture()
	{
		SAFE_RELEASE(m_resource);
		SAFE_RELEASE(m_shaderResourceView);
	}

	std::shared_ptr<Texture> Texture::Create(const char* path)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();

		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		if (blobLoader)
		{
			texture->initialize(s_textureLoader, blobLoader->createBlobHolder(path));
		}
		
		return texture;
	}
}