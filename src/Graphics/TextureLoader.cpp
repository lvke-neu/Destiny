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
#include "Engine/FileSystem.h"

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
			hr = DirectX::CreateDDSTextureFromMemory(Engine::GetInstance()->getGraphicsSystem()->getDevice(), (unsigned char*)blob->getData(), blob->getLength(), (ID3D11Resource**)&std::static_pointer_cast<Texture>(asset)->m_resource, &std::static_pointer_cast<Texture>(asset)->m_shaderResourceView);
		}
		else
		{
			hr = DirectX::CreateWICTextureFromMemory(Engine::GetInstance()->getGraphicsSystem()->getDevice(), (unsigned char*)blob->getData(), blob->getLength(), (ID3D11Resource**)&std::static_pointer_cast<Texture>(asset)->m_resource, &std::static_pointer_cast<Texture>(asset)->m_shaderResourceView);
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
		auto creationParam = std::static_pointer_cast<TextureCreationParam>(asset->getCreationParam());
		switch (creationParam->m_type)
		{
		case TextureCreationParam::Create2DSRV:
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

				hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateShaderResourceView(std::static_pointer_cast<Texture>(asset)->m_resource, &srvDesc, &std::static_pointer_cast<Texture>(asset)->m_shaderResourceView);
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
		case TextureCreationParam::Create2DUAV:
		{
			CD3D11_TEXTURE2D_DESC textureDesc((DXGI_FORMAT)creationParam->format, creationParam->width, creationParam->height, 1, 1);
			textureDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

			ID3D11Texture2D* texture2d = nullptr;
			HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateTexture2D(&textureDesc, nullptr, &texture2d);
			std::static_pointer_cast<Texture>(asset)->m_resource = texture2d;
			if (SUCCEEDED(hr))
			{
				
				CD3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc(D3D11_UAV_DIMENSION_TEXTURE2D, (DXGI_FORMAT)creationParam->format);

				hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateUnorderedAccessView(std::static_pointer_cast<Texture>(asset)->m_resource, &uavDesc, &std::static_pointer_cast<Texture>(asset)->m_unorderedAccessView);
				if (SUCCEEDED(hr))
				{

				}
				else
				{
					LOG_ERROR("LoadFromMemory CreateUnorderedAccessView failed");
					asset->loadFailed__();
					return;
				}

				CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, (DXGI_FORMAT)creationParam->format);
				hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateShaderResourceView(std::static_pointer_cast<Texture>(asset)->m_resource, &srvDesc, &std::static_pointer_cast<Texture>(asset)->m_shaderResourceView);
				if (SUCCEEDED(hr))
				{
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
		case TextureCreationParam::CreateStructured:
		{
			D3D11_BUFFER_DESC bufferDesc = {};
			bufferDesc.ByteWidth = creationParam->structuredBufferByteWidth;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			bufferDesc.StructureByteStride = creationParam->structuredBufferByteStride;

			D3D11_SUBRESOURCE_DATA initData = {};
			initData.pSysMem = creationParam->data ? initData.pSysMem = creationParam->data->getData() : nullptr;

			ID3D11Buffer* pBuffer = nullptr;
			HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBuffer(&bufferDesc, creationParam->data ? &initData : nullptr, &pBuffer);
			if (SUCCEEDED(hr))
			{
				std::static_pointer_cast<Texture>(asset)->m_resource = pBuffer;

				D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
				uavDesc.Format = DXGI_FORMAT_UNKNOWN;
				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				uavDesc.Buffer.NumElements = creationParam->structuredBufferByteWidth / creationParam->structuredBufferByteStride;
				uavDesc.Buffer.Flags = 0;
				//D3D11_BUFFER_UAV_FLAG_APPEND
				hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateUnorderedAccessView(std::static_pointer_cast<Texture>(asset)->m_resource, &uavDesc, &std::static_pointer_cast<Texture>(asset)->m_unorderedAccessView);
				if (SUCCEEDED(hr))
				{

				}
				else
				{
					LOG_ERROR("LoadFromMemory CreateStructured CreateUnorderedAccessView failed");
					asset->loadFailed__();
					return;
				}

				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
				srvDesc.Format = DXGI_FORMAT_UNKNOWN;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
				srvDesc.Buffer.NumElements = creationParam->structuredBufferByteWidth / creationParam->structuredBufferByteStride;
				srvDesc.Buffer.FirstElement = 0;

				hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateShaderResourceView(std::static_pointer_cast<Texture>(asset)->m_resource, &srvDesc, &std::static_pointer_cast<Texture>(asset)->m_shaderResourceView);
				if (SUCCEEDED(hr))
				{
					asset->loadSucceeded__();
				}
				else
				{
					LOG_ERROR("LoadFromMemory CreateStructured  CreateShaderResourceView failed");
					asset->loadFailed__();
				}
			}
			else
			{
				LOG_ERROR("LoadFromMemory CreateStructured Buffer failed");
				asset->loadFailed__();
			}

			break;
		}
		case TextureCreationParam::CreateTyped:
		{
			D3D11_BUFFER_DESC bufferDesc = {};
			bufferDesc.ByteWidth = creationParam->typedBufferByteWidth;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			//bufferDesc.StructureByteStride = creationParam->structureByteStride;

			D3D11_SUBRESOURCE_DATA initData = {};
			initData.pSysMem = creationParam->data ? initData.pSysMem = creationParam->data->getData() : nullptr;

			ID3D11Buffer* pBuffer = nullptr;
			HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBuffer(&bufferDesc, creationParam->data ? &initData : nullptr, &pBuffer);
			if (SUCCEEDED(hr))
			{
				std::static_pointer_cast<Texture>(asset)->m_resource = pBuffer;

				D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
				uavDesc.Format = (DXGI_FORMAT)creationParam->format;
				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				uavDesc.Buffer.NumElements = creationParam->typedBufferByteWidth / creationParam->structuredBufferByteStride;
				uavDesc.Buffer.Flags = 0;
				//D3D11_BUFFER_UAV_FLAG_APPEND
				hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateUnorderedAccessView(std::static_pointer_cast<Texture>(asset)->m_resource, &uavDesc, &std::static_pointer_cast<Texture>(asset)->m_unorderedAccessView);
				if (SUCCEEDED(hr))
				{

				}
				else
				{
					LOG_ERROR("LoadFromMemory CreateTyped CreateUnorderedAccessView failed");
					asset->loadFailed__();
					return;
				}

				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
				srvDesc.Format = (DXGI_FORMAT)creationParam->format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
				srvDesc.Buffer.NumElements = creationParam->typedBufferByteWidth / creationParam->structuredBufferByteStride;
				srvDesc.Buffer.FirstElement = 0;

				hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateShaderResourceView(std::static_pointer_cast<Texture>(asset)->m_resource, &srvDesc, &std::static_pointer_cast<Texture>(asset)->m_shaderResourceView);
				if (SUCCEEDED(hr))
				{
					asset->loadSucceeded__();
				}
				else
				{
					LOG_ERROR("LoadFromMemory CreateTyped  CreateShaderResourceView failed");
					asset->loadFailed__();
				}
			}
			else
			{
				LOG_ERROR("LoadFromMemory CreateTyped Buffer failed");
				asset->loadFailed__();
			}

			break;
		}
		case TextureCreationParam::CreateRaw:
		{
			//for D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS
			if (creationParam->rawBufferWidth < 20)
			{
				creationParam->rawBufferWidth = 20;
			}
			D3D11_BUFFER_DESC bufferDesc = {};
			bufferDesc.ByteWidth = creationParam->rawBufferWidth;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS | D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
			//bufferDesc.StructureByteStride = creationParam->structureByteStride;

			D3D11_SUBRESOURCE_DATA initData = {};
			initData.pSysMem = creationParam->data ? initData.pSysMem = creationParam->data->getData() : nullptr;

			ID3D11Buffer* pBuffer = nullptr;
			HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBuffer(&bufferDesc, creationParam->data ? &initData : nullptr, &pBuffer);
			if (SUCCEEDED(hr))
			{
				std::static_pointer_cast<Texture>(asset)->m_resource = pBuffer;

				D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
				uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				uavDesc.Buffer.NumElements = creationParam->rawBufferWidth / 4;
				uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
				//D3D11_BUFFER_UAV_FLAG_APPEND
				hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateUnorderedAccessView(std::static_pointer_cast<Texture>(asset)->m_resource, &uavDesc, &std::static_pointer_cast<Texture>(asset)->m_unorderedAccessView);
				if (SUCCEEDED(hr))
				{

				}
				else
				{
					LOG_ERROR("LoadFromMemory CreateRaw CreateUnorderedAccessView failed");
					asset->loadFailed__();
					return;
				}

				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
				srvDesc.Format = DXGI_FORMAT_R32_UINT;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
				srvDesc.Buffer.NumElements = creationParam->rawBufferWidth / 4;
				srvDesc.Buffer.FirstElement = 0;

				hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateShaderResourceView(std::static_pointer_cast<Texture>(asset)->m_resource, &srvDesc, &std::static_pointer_cast<Texture>(asset)->m_shaderResourceView);
				if (SUCCEEDED(hr))
				{
					asset->loadSucceeded__();
				}
				else
				{
					LOG_ERROR("LoadFromMemory CreateRaw  CreateShaderResourceView failed");
					asset->loadFailed__();
				}
			}
			else
			{
				LOG_ERROR("LoadFromMemory CreateRaw Buffer failed");
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
					hr = DirectX::SaveToDDSFile(
						cubeImage.GetImages(),
						cubeImage.GetImageCount(),
						cubeImage.GetMetadata(),
						DirectX::DDS_FLAGS_NONE,
						Utility::MultiByte2WideChar(FileSystem::ReplaceFileSuffixAddFileName(normalizedPath, "Cube", ".dds")).c_str()
					);

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
				DirectX::ScratchImage cubeImage;
				HDRTextureLoader::GenerateCubeImage(cubeImage, srcImage);

				DirectX::ScratchImage irradianceImage;
				bool loadSucceedIrradiance= false;
				HDRTextureLoader::GenerateIrradianceImage(irradianceImage, cubeImage);
				HDRTextureLoader::ConvertIrradianceImageToTexture(loadSucceedIrradiance, std::static_pointer_cast<Texture>(asset)->m_resource, std::static_pointer_cast<Texture>(asset)->m_shaderResourceView, irradianceImage);

				if (loadSucceedIrradiance)
				{
					hr = DirectX::SaveToDDSFile(
						irradianceImage.GetImages(),
						irradianceImage.GetImageCount(),
						irradianceImage.GetMetadata(),
						DirectX::DDS_FLAGS_NONE,
						Utility::MultiByte2WideChar(FileSystem::ReplaceFileSuffixAddFileName(normalizedPath, "Irradiance", ".dds")).c_str()
					);

					asset->getCreationParam().reset();
					asset->loadSucceeded__();
				}
				else
				{
					asset->loadFailed__();
				}
			}
			else if (creationParam->m_type == HdrCreationParma::CreateTextureType::Prefilter)
			{
				DirectX::ScratchImage cubeImage;
				HDRTextureLoader::GenerateCubeImage(cubeImage, srcImage);

				DirectX::ScratchImage prefilterImage;
				bool loadSucceedPrefilter = false;
				HDRTextureLoader::GeneratePrefilterMap(prefilterImage, cubeImage);
				HDRTextureLoader::ConvertIrradianceImageToTexture(loadSucceedPrefilter, std::static_pointer_cast<Texture>(asset)->m_resource, std::static_pointer_cast<Texture>(asset)->m_shaderResourceView, prefilterImage);

				if (loadSucceedPrefilter)
				{
					hr = DirectX::SaveToDDSFile(
						prefilterImage.GetImages(),
						prefilterImage.GetImageCount(),
						prefilterImage.GetMetadata(),
						DirectX::DDS_FLAGS_NONE,
						Utility::MultiByte2WideChar(FileSystem::ReplaceFileSuffixAddFileName(normalizedPath, "Prefilter", ".dds")).c_str()
					);
					asset->getCreationParam().reset();
					asset->loadSucceeded__();
				}
				else
				{
					asset->loadFailed__();
				}
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