#include "TextureLoader_dds.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"
#include "Engine/Utility.h"
#include "Texture.h"
#include "DDSTextureLoader.h"
#include "GraphicsSystem.h"

namespace Destiny
{
	TextureLoader_dds::TextureLoader_dds()
	{

	}

	TextureLoader_dds::~TextureLoader_dds()
	{

	}

	void TextureLoader_dds::loadAsset(std::shared_ptr<Asset> asset)
	{
		m_mtx.lock();
		if (!asset || !asset->getBlobHolder() || !asset->getBlobHolder()->getBlob())
		{
			asset->loadFailed__();
			m_mtx.unlock();
			return;
		}

		std::string path((char*)asset->getBlobHolder()->getBlob()->getData(), asset->getBlobHolder()->getBlob()->getLength());
		HRESULT hr = DirectX::CreateDDSTextureFromFile(Engine::GetInstance()->getGraphicsSystem()->getDevice(), multiByteToWideChar(path), &((Texture*)asset.get())->m_resource, &((Texture*)asset.get())->m_shaderResourceView);
		if (SUCCEEDED(hr))
		{
			asset->loadSucceeded__();
		}
		else
		{
			asset->loadFailed__();
		}
		m_mtx.unlock();
	}

	std::shared_ptr<Texture> TextureLoader_dds::createAsset(std::shared_ptr<BlobHolder> blobHolder)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->initialize(shared_from_this(), blobHolder);
		return texture;
	}
}