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

	DirectX::XMFLOAT2 DirectionToPanoramaUV(const DirectX::XMFLOAT3& direction)
	{
		float phi = atan2f(direction.z, direction.x);         
		float theta = acosf(direction.y);                     

		float u = 0.5f + phi / (2.0f * DirectX::XM_PI);
		float v = theta / DirectX::XM_PI;

		return { u, v };
	}

	template<typename T>
	T clamp(T value, T min, T max) 
	{
		if (value < min)
		{
			return min;
		}

		if (value > max)
		{
			return max;
		}

		return value;
	}

	DirectX::XMFLOAT4 SamplePanorama(const DirectX::ScratchImage& panorama, float u, float v) 
	{
		const DirectX::Image* img = panorama.GetImage(0, 0, 0);
		if (!img)
		{
			return { 0, 0, 0, 1 };
		}
		
		u = fmodf(u, 1.0f);
		if (u < 0) u += 1.0f;
		v = clamp(v, 0.0f, 1.0f);

		float x = u * (img->width - 1);
		float y = v * (img->height - 1);

		int x0 = static_cast<int>(x);
		int y0 = static_cast<int>(y);
		int x1 = std::min(x0 + 1, static_cast<int>(img->width - 1));
		int y1 = std::min(y0 + 1, static_cast<int>(img->height - 1));

		float dx = x - x0;
		float dy = y - y0;

		auto GetPixel = [&](int x, int y) -> DirectX::XMFLOAT4
		{
			const float* pixel = reinterpret_cast<const float*>(img->pixels + y * img->rowPitch + x * 4 * sizeof(float));
			return { pixel[0], pixel[1], pixel[2], pixel[3] };
		};

		DirectX::XMFLOAT4 c00 = GetPixel(x0, y0);
		DirectX::XMFLOAT4 c10 = GetPixel(x1, y0);
		DirectX::XMFLOAT4 c01 = GetPixel(x0, y1);
		DirectX::XMFLOAT4 c11 = GetPixel(x1, y1);

		DirectX::XMFLOAT4 result;
		result.x = c00.x * (1 - dx) * (1 - dy) + c10.x * dx * (1 - dy) + c01.x * (1 - dx) * dy + c11.x * dx * dy;
		result.y = c00.y * (1 - dx) * (1 - dy) + c10.y * dx * (1 - dy) + c01.y * (1 - dx) * dy + c11.y * dx * dy;
		result.z = c00.z * (1 - dx) * (1 - dy) + c10.z * dx * (1 - dy) + c01.z * (1 - dx) * dy + c11.z * dx * dy;
		result.w = c00.w * (1 - dx) * (1 - dy) + c10.w * dx * (1 - dy) + c01.w * (1 - dx) * dy + c11.w * dx * dy;

		return result;
	}

	void convertSrcHdrImageToCubeImage(DirectX::ScratchImage& cubeImage, const DirectX::ScratchImage& srcImage)
	{
		const DirectX::XMFLOAT3 faceDirections[6][3] =
		{

			{ {1,0,0}, {0,1,0}, {0,0,1} },
			{ {-1,0,0},{0,1,0}, {0,0,-1} },
			{ {1,0,0}, {0,0,1}, {0,1,0} },
			{ {1,0,0}, {0,0,-1},{0,-1,0}},
			{ {0,0,1}, {0,1,0}, {-1,0,0} },
			{ {0,0,-1},{0,1,0}, {1,0,0} }
		};

		size_t cubemapSize = 1024;

		DirectX::TexMetadata meta = {};
		meta.format = srcImage.GetMetadata().format;
		meta.width = cubemapSize;
		meta.height = cubemapSize;
		meta.depth = 1;
		meta.arraySize = 6;
		meta.mipLevels = 1;
		meta.dimension = DirectX::TEX_DIMENSION_TEXTURE2D;
		meta.miscFlags = DirectX::TEX_MISC_TEXTURECUBE;

		cubeImage.Initialize(meta);

		for (size_t face = 0; face < 6; face++)
		{
			const DirectX::Image* destImage = cubeImage.GetImage(0, face, 0);
			const DirectX::XMFLOAT3& right = faceDirections[face][0];
			const DirectX::XMFLOAT3& up = faceDirections[face][1];
			const DirectX::XMFLOAT3& forward = faceDirections[face][2];

			for (size_t y = 0; y < cubemapSize; y++) {
				for (size_t x = 0; x < cubemapSize; x++) {

					float u = 2.0f * ((float)x / (cubemapSize - 1)) - 1.0f;
					float v = 1.0f - 2.0f * ((float)y / (cubemapSize - 1));

					if (face == 2 || face == 3)
					{
						float temp = u;
						u = v;
						v = temp;

						if (face == 3) {
							u = -u;
							v = -v;
						}
					}

					DirectX::XMFLOAT3 direction
					(
						right.x * u + up.x * v + forward.x,
						right.y * u + up.y * v + forward.y,
						right.z * u + up.z * v + forward.z
					);

					DirectX::XMVECTOR dirVec = XMLoadFloat3(&direction);
					dirVec = DirectX::XMVector3Normalize(dirVec);
					XMStoreFloat3(&direction, dirVec);
					DirectX::XMFLOAT2 uv = DirectionToPanoramaUV(direction);
					DirectX::XMFLOAT4 color = SamplePanorama(srcImage, uv.x, uv.y);

					float* pixel = reinterpret_cast<float*>(destImage->pixels + y * destImage->rowPitch + x * 4 * sizeof(float));
					pixel[0] = color.x;
					pixel[1] = color.y;
					pixel[2] = color.z;
					pixel[3] = color.w;
				}
			}
		}	
	}

	void TextureLoader::loadFromHDR(std::shared_ptr<Asset> asset)
	{
		auto creationParam = std::static_pointer_cast<BlobHolder>(asset->getCreationParam());
		auto blob = creationParam->getBlob();

		HRESULT hr = 0;

		DirectX::ScratchImage srcImage;
		DirectX::LoadFromHDRMemory(blob->getData(), blob->getLength(), nullptr, srcImage);

		if (SUCCEEDED(hr))
		{
			DirectX::ScratchImage cubeImage;

			convertSrcHdrImageToCubeImage(cubeImage, srcImage);

			const auto& cubeMetadata = cubeImage.GetMetadata();
			CD3D11_TEXTURE2D_DESC textureDesc(cubeMetadata.format, (unsigned int)cubeMetadata.width, (unsigned int)cubeMetadata.height, 6, 1, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT, 0, 1, 0, D3D11_RESOURCE_MISC_TEXTURECUBE);

			std::vector<D3D11_SUBRESOURCE_DATA> initData;
			initData.resize(cubeMetadata.arraySize * cubeMetadata.mipLevels);

			for (size_t item = 0; item < cubeMetadata.arraySize; ++item) 
			{
				for (size_t level = 0; level < cubeMetadata.mipLevels; ++level) 
				{
					const DirectX::Image* img = cubeImage.GetImage(level, item, 0);

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
					asset->loadFailed__();
					LOG_ERROR("LoadFromHDR CreateShaderResourceView failed");
				}
			}
			else
			{
				asset->loadFailed__();
				LOG_ERROR("LoadFromHDR CreateTexture2D failed");
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