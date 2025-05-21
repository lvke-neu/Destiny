#include "TextureLoader.h"
#include "Texture.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "HDRTextureLoader.h"
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
		if (asset->isLoadingSucceed())
		{
			return;
		}

		if (!asset || !std::dynamic_pointer_cast<Texture>(asset))
		{
			asset->loadFailed__();
			return;
		}

		if (std::dynamic_pointer_cast<BlobHolder>(asset->getCreationParam()))
		{
			loadFromPath(asset);
			return;
		}
		
		if (std::dynamic_pointer_cast<TextureCreationParam>(asset->getCreationParam()))
		{
			loadFromMemory(asset);
			return;
		}

		if (std::dynamic_pointer_cast<HdrCreationParma>(asset->getCreationParam()))
		{
			loadFromHDR(asset);
			return;
		}
	}

	void TextureLoader::loadFromPath(std::shared_ptr<Asset> asset)
	{
		auto creationParam = std::dynamic_pointer_cast<BlobHolder>(asset->getCreationParam());

		if (creationParam->isLoadingPending())
		{
			creationParam->load(0);
		}

		auto blob = creationParam->getBlob();
		if (creationParam->isLoadingFailed() || !blob)
		{
			asset->loadFailed__();
			return;
		}

		if (!creationParam->getBlobLoader())
		{
			asset->loadFailed__();
			return;
		}

		HRESULT hr = 0;
		auto normalizedPath = creationParam->getBlobLoader()->normalizedPath(creationParam);

		if (normalizedPath.find(".dds") != std::string::npos)
		{
			hr = DirectX::CreateDDSTextureFromMemory(Engine::GetInstance()->getGraphicsSystem()->getDevice(), (unsigned char*)blob->getData(), blob->getLength(), (ID3D11Resource**)&std::dynamic_pointer_cast<Texture>(asset)->m_resource, &std::dynamic_pointer_cast<Texture>(asset)->m_shaderResourceView);
		}
		else
		{
			hr = DirectX::CreateWICTextureFromMemory(Engine::GetInstance()->getGraphicsSystem()->getDevice(), (unsigned char*)blob->getData(), blob->getLength(), (ID3D11Resource**)&std::dynamic_pointer_cast<Texture>(asset)->m_resource, &std::dynamic_pointer_cast<Texture>(asset)->m_shaderResourceView);
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

	void TextureLoader::loadFromMemory(std::shared_ptr<Asset> asset)
	{
		auto creationParam = std::dynamic_pointer_cast<TextureCreationParam>(asset->getCreationParam());
		switch (creationParam->m_type)
		{
		case TextureCreationParam::Create2D:
		{
			CD3D11_TEXTURE2D_DESC textureDesc((DXGI_FORMAT)creationParam->format, creationParam->width, creationParam->height, 1, 1);
	
			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = creationParam->data->getData();
			data.SysMemPitch = creationParam->pitch;
			data.SysMemSlicePitch = creationParam->slicePitch;

			ID3D11Texture2D* texture2d = nullptr;
			HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateTexture2D(&textureDesc, &data, &texture2d);
			std::static_pointer_cast<Texture>(asset)->m_resource = texture2d;
			if (SUCCEEDED(hr))
			{
				CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, (DXGI_FORMAT)creationParam->format);

				hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateShaderResourceView(std::dynamic_pointer_cast<Texture>(asset)->m_resource, &srvDesc, &std::dynamic_pointer_cast<Texture>(asset)->m_shaderResourceView);
				if (SUCCEEDED(hr))
				{
					asset->getCreationParam().reset();
					asset->loadSucceeded__();
				}
				else
				{
					LOG_ERROR("LoadFromMemory CreateShaderResourceView failed");
					asset->loadFailed__();
				}
			}
			else
			{
				LOG_ERROR("LoadFromMemory CreateTexture2D failed");
				asset->loadFailed__();
			}

			break;
		}
		default:
		{
			LOG_ERROR("Texture load failed : CreateTextureType Invalid");
			asset->loadFailed__();
			break;
		}

		}
		
	}

	void TextureLoader::loadFromHDR(std::shared_ptr<Asset> asset)
	{
		auto creationParam = std::dynamic_pointer_cast<HdrCreationParma>(asset->getCreationParam());

		if (creationParam->m_blobHolder->isLoadingPending())
		{
			creationParam->m_blobHolder->load(0);
		}

		auto blob = creationParam->m_blobHolder->getBlob();
		if (creationParam->m_blobHolder->isLoadingFailed() || !blob)
		{
			asset->loadFailed__();
			return;
		}

		if (!creationParam->m_blobHolder->getBlobLoader())
		{
			asset->loadFailed__();
			return;
		}

		auto normalizedPath = creationParam->m_blobHolder->getBlobLoader()->normalizedPath(creationParam->m_blobHolder);

		HRESULT hr = 0;

		DirectX::ScratchImage srcImage;
		hr = DirectX::LoadFromHDRMemory(blob->getData(), blob->getLength(), nullptr, srcImage);

		if (SUCCEEDED(hr))
		{
			if (creationParam->m_type == HdrCreationParma::CreateTextureType::Hdr)
			{
				DirectX::ScratchImage cubeImage;
				HDRTextureLoader::GenerateCubeImage(cubeImage, srcImage);
				bool loadSucceedCube = false;
				HDRTextureLoader::ConvertCubeImageToTexture(loadSucceedCube, std::static_pointer_cast<Texture>(asset)->m_resource, std::static_pointer_cast<Texture>(asset)->m_shaderResourceView, cubeImage);

				if (loadSucceedCube)
				{
					asset->getCreationParam().reset();
					asset->loadSucceeded__();
				}
				else
				{
					asset->loadFailed__();
				}
			}
			else if (creationParam->m_type == HdrCreationParma::CreateTextureType::Irradiance)
			{

			}
			else if (creationParam->m_type == HdrCreationParma::CreateTextureType::Prefilter)
			{

			}
			else
			{
				asset->loadFailed__();
				LOG_ERROR("Hdr Texture load failed : {0}, unsupported type", normalizedPath);
			}
		}
		else
		{
			asset->loadFailed__();
			LOG_ERROR("Texture loadFromHDRMemory failed : {0}, LoadFromHDRMemory ", normalizedPath);
		}
	}
}