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
#include "ThirdParty/stb_image/stb_image.h"
#include "ThirdParty/DirectXTex/DirectXTex.h"

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
		else if (normalizedPath.find(".hdr") != std::string::npos)
		{
			loadFromHDR(asset);
			return;
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

	size_t GetBytesPerPixel(DXGI_FORMAT format)
	{
		switch (format)
		{
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_R8_SNORM:
		case DXGI_FORMAT_R8_UINT:
		case DXGI_FORMAT_R8_SINT:
			return 1;

		case DXGI_FORMAT_R16_UNORM:
		case DXGI_FORMAT_R16_SNORM:
		case DXGI_FORMAT_R16_UINT:
		case DXGI_FORMAT_R16_SINT:
		case DXGI_FORMAT_R16_FLOAT:
		case DXGI_FORMAT_D16_UNORM:
		case DXGI_FORMAT_R8G8_UNORM:
		case DXGI_FORMAT_R8G8_SNORM:
		case DXGI_FORMAT_R8G8_UINT:
		case DXGI_FORMAT_R8G8_SINT:
			return 2;

		case DXGI_FORMAT_R32_UINT:
		case DXGI_FORMAT_R32_SINT:
		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R8G8B8A8_SNORM:
		case DXGI_FORMAT_R8G8B8A8_UINT:
		case DXGI_FORMAT_R8G8B8A8_SINT:
		case DXGI_FORMAT_B8G8R8A8_UNORM:
		case DXGI_FORMAT_B8G8R8X8_UNORM:
		case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
		case DXGI_FORMAT_R10G10B10A2_UNORM:
		case DXGI_FORMAT_R10G10B10A2_UINT:
		case DXGI_FORMAT_R11G11B10_FLOAT:
		case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			return 4;

		case DXGI_FORMAT_R32G32_FLOAT:
		case DXGI_FORMAT_R32G32_UINT:
		case DXGI_FORMAT_R32G32_SINT:
			return 8;

		case DXGI_FORMAT_R32G32B32_FLOAT:
		case DXGI_FORMAT_R32G32B32_UINT:
		case DXGI_FORMAT_R32G32B32_SINT:
			return 12;

		case DXGI_FORMAT_R32G32B32A32_FLOAT:
		case DXGI_FORMAT_R32G32B32A32_UINT:
		case DXGI_FORMAT_R32G32B32A32_SINT:
			return 16;

		default:
			assert(false && "Unsupported format");
			return 0;
		}
	}

	void TextureLoader::loadFromHDR(std::shared_ptr<Asset> asset)
	{
		auto creationParam = std::dynamic_pointer_cast<BlobHolder>(asset->getCreationParam());
		auto blob = creationParam->getBlob();
	
		DirectX::ScratchImage srcImage;
		DirectX::LoadFromHDRMemory(blob->getData(), blob->getLength(), nullptr, srcImage);

		HRESULT hr = 0;
		if (SUCCEEDED(hr)) 
		{
			auto srcMetadata = srcImage.GetMetadata();

			size_t cubeSize = 1024;
			DirectX::ScratchImage cubeImage;
			cubeImage.InitializeCube(srcMetadata.format, cubeSize, cubeSize, 1, 1);

			using namespace DirectX;

			const float aspectRatio = static_cast<float>(srcMetadata.width) / srcMetadata.height;
			const float fov = XM_PIDIV2;

			for (int face = 0; face < 6; ++face)
			{
				const Image* cubeFace = &cubeImage.GetImages()[face];

				for (size_t y = 0; y < cubeSize; ++y)
				{
					for (size_t x = 0; x < cubeSize; ++x)
					{
						float u = (x + 0.5f) / cubeSize;
						float v = (y + 0.5f) / cubeSize;

						XMFLOAT3 direction;
						switch (face)
						{
						case 0:
							direction = XMFLOAT3(1.0f, -v * 2.0f + 1.0f, -u * 2.0f + 1.0f);
							break;
						case 1:
							direction = XMFLOAT3(-1.0f, -v * 2.0f + 1.0f, u * 2.0f - 1.0f);
							break;
						case 2:
							direction = XMFLOAT3(u * 2.0f - 1.0f, 1.0f, -v * 2.0f + 1.0f);
							break;
						case 3:
							direction = XMFLOAT3(-u * 2.0f + 1.0f, -1.0f, -v * 2.0f + 1.0f);
							break;
						case 4:
							direction = XMFLOAT3(u * 2.0f - 1.0f, -v * 2.0f + 1.0f, 1.0f);
							break;
						case 5:
							direction = XMFLOAT3(-u * 2.0f + 1.0f, -v * 2.0f + 1.0f, -1.0f);
							break;
						}

						XMVECTOR dirVec = XMLoadFloat3(&direction);
						dirVec = XMVector3Normalize(dirVec);
						XMStoreFloat3(&direction, dirVec);

						float theta = atan2f(direction.z, direction.x);
						float phi = acosf(direction.y);

						float uHdr = (theta + XM_PI) / (2 * XM_PI);
						float vHdr = phi / XM_PI;

						uHdr = std::max(0.0f, std::min(1.0f, uHdr));
						vHdr = std::max(0.0f, std::min(1.0f, vHdr));

						size_t x0 = static_cast<size_t>(uHdr * (srcMetadata.width - 1));
						size_t y0 = static_cast<size_t>(vHdr * (srcMetadata.height - 1));
						size_t x1 = std::min(x0 + 1, srcMetadata.width - 1);
						size_t y1 = std::min(y0 + 1, srcMetadata.height - 1);

						float wx = uHdr * (srcMetadata.width - 1) - x0;
						float wy = vHdr * (srcMetadata.height - 1) - y0;

						const uint8_t* p00 = srcImage.GetImages()->pixels + (y0 * srcImage.GetImages()->rowPitch) + (x0 * GetBytesPerPixel(srcMetadata.format));
						const uint8_t* p01 = srcImage.GetImages()->pixels + (y0 * srcImage.GetImages()->rowPitch) + (x1 * GetBytesPerPixel(srcMetadata.format));
						const uint8_t* p10 = srcImage.GetImages()->pixels + (y1 * srcImage.GetImages()->rowPitch) + (x0 * GetBytesPerPixel(srcMetadata.format));
						const uint8_t* p11 = srcImage.GetImages()->pixels + (y1 * srcImage.GetImages()->rowPitch) + (x1 * GetBytesPerPixel(srcMetadata.format));

						size_t bytesPerPixel = GetBytesPerPixel(srcMetadata.format);
						uint8_t* dstPixel = cubeFace->pixels + (y * cubeFace->rowPitch) + (x * bytesPerPixel);

						for (size_t c = 0; c < bytesPerPixel; ++c)
						{
							float val00 = static_cast<float>(p00[c]);
							float val01 = static_cast<float>(p01[c]);
							float val10 = static_cast<float>(p10[c]);
							float val11 = static_cast<float>(p11[c]);

							float val0 = val00 * (1.0f - wx) + val01 * wx;
							float val1 = val10 * (1.0f - wx) + val11 * wx;

							float val = val0 * (1.0f - wy) + val1 * wy;

							dstPixel[c] = static_cast<uint8_t>(std::max(0.0f, std::min(255.0f, val)));
						}
					}
				}
			}
			
			auto cubeMetadata = cubeImage.GetMetadata();
			CD3D11_TEXTURE2D_DESC textureDesc(cubeMetadata.format, (unsigned int)cubeMetadata.width, (unsigned int)cubeMetadata.height, 6, 1, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT, 0, 1, 0, D3D11_RESOURCE_MISC_TEXTURECUBE);

			std::vector<D3D11_SUBRESOURCE_DATA> initData;
			initData.resize(cubeMetadata.arraySize * cubeMetadata.mipLevels);

			for (size_t item = 0; item < cubeMetadata.arraySize; ++item) {
				for (size_t level = 0; level < cubeMetadata.mipLevels; ++level) {
					const Image* img = cubeImage.GetImage(level, item, 0);

					size_t index = item * cubeMetadata.mipLevels + level;
					initData[index].pSysMem = img->pixels;
					initData[index].SysMemPitch = (unsigned int)img->rowPitch;
					initData[index].SysMemSlicePitch = (unsigned int)img->slicePitch;
				}
			}

			ID3D11Texture2D* texture2d = nullptr;
			hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateTexture2D(&textureDesc, initData.data(), &texture2d);
			std::static_pointer_cast<Texture>(asset)->m_resource = texture2d;
			if (SUCCEEDED(hr))
			{
				CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURECUBE, cubeMetadata.format, 0, 1);

				hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateShaderResourceView(texture2d, &srvDesc, &std::dynamic_pointer_cast<Texture>(asset)->m_shaderResourceView);
				if (SUCCEEDED(hr))
				{
					asset->getCreationParam().reset();
					asset->loadSucceeded__();
				}
				else
				{
					LOG_ERROR("loadFromHDR CreateShaderResourceView failed");
					asset->loadFailed__();
				}
			}
			else
			{
				LOG_ERROR("loadFromHDR CreateTexture2D failed");
				asset->loadFailed__();
			}
		}
		else
		{
			asset->loadFailed__();
			auto normalizedPath = creationParam->getBlobLoader()->normalizedPath(creationParam);
			LOG_ERROR("Texture load failed : {0}", normalizedPath);

		}
	}
}