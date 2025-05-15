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
	std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::s_cache;
	Texture::Texture() :
		m_resource(nullptr),
		m_shaderResourceView(nullptr)
	{

	}

	Texture::Texture(ID3D11Resource* resource, ID3D11ShaderResourceView* shaderResourceView) :
		m_resource(resource),
		m_shaderResourceView(shaderResourceView)
	{
		SAFE_ADDREF(m_resource);
		SAFE_ADDREF(m_shaderResourceView);
	}

	Texture::~Texture()
	{
		SAFE_RELEASE(m_resource);
		SAFE_RELEASE(m_shaderResourceView);
	}

	std::shared_ptr<Texture> Texture::Create(const char* path)
	{
		auto iter = s_cache.find(path);
		if (iter != s_cache.end())
		{
			return iter->second;
		}

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();

		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		if (blobLoader)
		{
			texture->initialize(s_textureLoader, blobLoader->createBlobHolder(path));
		}

		s_cache.insert({ path, texture });
		return texture;
	}

	std::shared_ptr<Texture> Texture::Create2D(int format, unsigned int width, unsigned int height, std::shared_ptr<Blob> data, unsigned int pitch, unsigned int slicePitch)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		std::shared_ptr<TextureCreationParam> creationParam = std::make_shared<TextureCreationParam>();
		
		creationParam->m_type = TextureCreationParam::Create2D;
		creationParam->format = format;
		creationParam->width = width;
		creationParam->height = height;
		creationParam->data = data;
		creationParam->pitch = pitch;
		creationParam->slicePitch = slicePitch;
		texture->initialize(s_textureLoader, creationParam);

		return texture;
	}

	std::string Texture::getPath()
	{
		std::string path;

		if (std::dynamic_pointer_cast<BlobHolder>(m_creationParam))
		{
			return std::static_pointer_cast<BlobHolder>(m_creationParam)->getPath();
		}
		else if (std::dynamic_pointer_cast<TextureCreationParam>(m_creationParam))
		{
			auto creationParam = std::static_pointer_cast<TextureCreationParam>(m_creationParam);
			if (creationParam->m_type == TextureCreationParam::Create2D)
			{
				path += "Type:Create2D,";
			}
			path += "width:" + std::to_string(creationParam->width) + "height:" + std::to_string(creationParam->height);

		}

		return path;
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

	void Texture::unBind(std::shared_ptr<TextureDesc> desc)
	{
		if (!desc)
		{
			return;
		}

		ID3D11ShaderResourceView* srv = nullptr;
		for (const auto& textureBindFlag : desc->textureBindFlag)
		{
			if (textureBindFlag.second)
			{
				switch (textureBindFlag.first)
				{
				case TextureBindFlag::BindVS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetShaderResources(desc->startSlot, 1, &srv);
					break;
				case TextureBindFlag::BindPS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetShaderResources(desc->startSlot, 1, &srv);
					break;
				case TextureBindFlag::BindGS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->GSSetShaderResources(desc->startSlot, 1, &srv);
					break;
				}
			}
		}
	}
}