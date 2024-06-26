#include "TextureLoader.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"
#include "Engine/Utility.h"
#include "Texture.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "GraphicsSystem.h"


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
		m_mtx.lock();
		
		if (asset->isLoadingSucceed())
		{
			m_mtx.unlock();
			return;
		}

		if (!asset || !dynamic_cast<Texture*>(asset.get()))
		{
			asset->loadFailed__();
			m_mtx.unlock();
			return;
		}

		auto creationParam = dynamic_cast<BlobHolder*>(asset->getCreationParam().get());
		if (!creationParam)
		{
			asset->loadFailed__();
			m_mtx.unlock();
			return;
		}

		if (creationParam->isLoadingPending())
		{
			creationParam->load(0);
		}

		if (creationParam->isLoadingFailed() || !creationParam->getBlob())
		{
			asset->loadFailed__();
			m_mtx.unlock();
			return;
		}

		HRESULT hr = 0;

		if (creationParam->getFullPath().find(".dds") != std::string::npos)
		{
			hr = DirectX::CreateDDSTextureFromMemory(Engine::GetInstance()->getGraphicsSystem()->getDevice(), (unsigned char*)creationParam->getBlob()->getData(), creationParam->getBlob()->getLength(), &((Texture*)asset.get())->m_resource, &((Texture*)asset.get())->m_shaderResourceView);
		}
		else
		{
			hr = DirectX::CreateWICTextureFromMemory(Engine::GetInstance()->getGraphicsSystem()->getDevice(), (unsigned char*)creationParam->getBlob()->getData(), creationParam->getBlob()->getLength(), &((Texture*)asset.get())->m_resource, &((Texture*)asset.get())->m_shaderResourceView);
		}

		if (SUCCEEDED(hr))
		{
			asset->loadSucceeded__();
		}
		else
		{
			LOG_ERROR("Texture load failed : {0}", creationParam->getFullPath());
			asset->loadFailed__();
		}

		m_mtx.unlock();
	}
}