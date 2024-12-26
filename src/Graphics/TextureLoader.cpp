#include "TextureLoader.h"
#include "Texture.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "GraphicsSystem.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoader.h"
#include "Engine/Utility.h"

namespace Destiny
{
	TextureLoader::TextureLoader()
	{
		DirectX::WIC_INIT();
	}

	TextureLoader::~TextureLoader()
	{

	}

	void TextureLoader::loadAsset(std::shared_ptr<Asset> asset)
	{
		if (!asset || !std::dynamic_pointer_cast<Texture>(asset))
		{
			asset->loadFailed__();
			return;
		}

		if (asset->isLoadingSucceed())
		{
			return;
		}

		auto creationParam = std::dynamic_pointer_cast<BlobHolder>(asset->getCreationParam());
		if (!creationParam || ! creationParam->getBlobLoader())
		{
			asset->loadFailed__();
			return;
		}

		if (creationParam->isLoadingPending())
		{
			creationParam->load(0);
		}

		if (creationParam->isLoadingFailed() || !creationParam->getBlob())
		{
			asset->loadFailed__();
			return;
		}
		
		HRESULT hr = 0;
		auto normalizedPath = creationParam->getBlobLoader()->normalizedPath(creationParam);
		if (normalizedPath.find(".dds") != std::string::npos)
		{
			hr = DirectX::CreateDDSTextureFromMemory(Engine::GetInstance()->getGraphicsSystem()->getDevice(), (unsigned char*)creationParam->getBlob()->getData(), creationParam->getBlob()->getLength(), &((Texture*)asset.get())->m_resource, &((Texture*)asset.get())->m_shaderResourceView);
		}
		else
		{
			hr = DirectX::CreateWICTextureFromMemory(Engine::GetInstance()->getGraphicsSystem()->getDevice(), (unsigned char*)creationParam->getBlob()->getData(), creationParam->getBlob()->getLength(), &((Texture*)asset.get())->m_resource, &((Texture*)asset.get())->m_shaderResourceView);
		}
		
		if (SUCCEEDED(hr))
		{
			asset->getCreationParam().reset();
			asset->loadSucceeded__();
		}
		else
		{
			LOG_ERROR("Texture load failed : {0}", normalizedPath);
			asset->loadFailed__();
		}
	}
}