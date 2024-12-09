#include "Texture.h"
#include "TextureLoader.h"
#include "GraphicsSystem.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoader.h"
#include "Engine/Utility.h"
#include <d3d11.h>

namespace Destiny
{
	std::shared_ptr<TextureLoader> Texture::s_textureLoader = std::make_shared<TextureLoader>();
	std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::m_cache;
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
		auto iter = m_cache.find(path);
		if (iter != m_cache.end())
		{
			return m_cache[path];
		}

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();

		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		if (blobLoader)
		{
			texture->initialize(s_textureLoader, blobLoader->createBlobHolder(path));
		}
		
		m_cache[path] = texture;

		return texture;
	}

	void Texture::bind(std::shared_ptr<TextureDesc> desc)
	{
		if (!desc)
		{
			return;
		}

		for (const auto& textureBindFlag : desc->textureBindFlag)
		{
			if (textureBindFlag.second)
			{
				switch (textureBindFlag.first)
				{
				case TextureBindFlag::BindVS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetShaderResources(desc->startSlot, 1, &m_shaderResourceView);
					break;
				case TextureBindFlag::BindPS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetShaderResources(desc->startSlot, 1, &m_shaderResourceView);
					break;
				case TextureBindFlag::BindGS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->GSSetShaderResources(desc->startSlot, 1, &m_shaderResourceView);
					break;
				}
			}
		}
	}
}