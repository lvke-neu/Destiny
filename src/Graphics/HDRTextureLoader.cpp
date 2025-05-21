#include "HDRTextureLoader.h"
#include "Engine/Utility.h"
#include "Graphics/GraphicsSystem.h"
#include <mutex>
#undef max
#undef min

namespace Destiny
{
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

	void HDRTextureLoader::GenerateCubeImage(DirectX::ScratchImage& cubeImage, const DirectX::ScratchImage& srcImage, uint32_t threadCount)
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

		std::mutex mtx;
		std::vector<std::thread> threads;
		threads.reserve(threadCount);

		auto processFace = [&](uint32_t face)
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

			std::lock_guard<std::mutex> lock(mtx);
			LOG_INFO("Finished cubemap face {0}/6", std::to_string(face));
		};

		for (uint32_t face = 0; face < 6; face++)
		{
			threads.emplace_back(processFace, face);
			//if (threads.size() >= threadCount)
			//{
			//	threads.back().join();
			//	threads.pop_back();
			//}
		}

		for (auto& thread : threads)
			thread.join();

		LOG_TRACE("===================End=====================================");
	}

	void HDRTextureLoader::ConvertCubeImageToTexture(bool& loadSucceed, ID3D11Resource*& resource, ID3D11ShaderResourceView*& shaderResourceView, const DirectX::ScratchImage& cubeImage)
	{
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

		HRESULT hr = 0;
		ID3D11Texture2D* texture2d = nullptr;
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateTexture2D(&textureDesc, initData.data(), &texture2d);
		if (SUCCEEDED(hr))
		{
			CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURECUBE, cubeMetadata.format, 0, 1);
			hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateShaderResourceView(texture2d, &srvDesc, &shaderResourceView);
			if (SUCCEEDED(hr))
			{
				resource = texture2d;
				loadSucceed = true;
			}
			else
			{
				LOG_ERROR("LoadFromHDR CreateShaderResourceView failed");
				loadSucceed = false;
			}
		}
		else
		{
			LOG_ERROR("LoadFromHDR CreateTexture2D failed");
			loadSucceed = false;
		}
	}

	float radicalInverse(size_t bits)
	{
		bits = (bits << 16u) | (bits >> 16u);
		bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
		bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
		bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
		bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
		return float(bits) * 2.3283064365386963e-10f;
	}

	std::vector<DirectX::XMFLOAT3> generateSphericalSamples(size_t count)
	{
		std::vector<DirectX::XMFLOAT3> samples;
		samples.reserve(count);

		for (size_t i = 0; i < count; i++)
		{
			float xi1 = float(i) / float(count);
			float xi2 = radicalInverse(i);

			float phi = 2.0f * DirectX::XM_PI * xi1;
			float cosTheta = 1.0f - xi2;
			float sinTheta = sqrtf(1.0f - cosTheta * cosTheta);

			samples.push_back(DirectX::XMFLOAT3(
				cosf(phi) * sinTheta,
				cosTheta,
				sinf(phi) * sinTheta
			));
		}

		return samples;
	}

	DirectX::XMFLOAT2 directionToCubeUV(const DirectX::XMFLOAT3& direction)
	{
		float absX = fabsf(direction.x);
		float absY = fabsf(direction.y);
		float absZ = fabsf(direction.z);

		float maxComponent = fmaxf(fmaxf(absX, absY), absZ);
		float u, v;

		if (maxComponent == absX)
		{
			u = -direction.z / direction.x;
			v = direction.y / direction.x;
		}
		else if (maxComponent == absY)
		{
			u = direction.x / direction.y;
			v = direction.z / direction.y;
		}
		else
		{
			u = direction.x / direction.z;
			v = -direction.y / direction.z;
		}

		u = 0.5f * (u + 1.0f);
		v = 0.5f * (v + 1.0f);

		return { u, v };
	}

	size_t getCubeMapFace(const DirectX::XMFLOAT3& direction)
	{
		float absX = fabsf(direction.x);
		float absY = fabsf(direction.y);
		float absZ = fabsf(direction.z);

		if (absX >= absY && absX >= absZ)
			return direction.x > 0 ? 0 : 1; // +X, -X
		else if (absY >= absX && absY >= absZ)
			return direction.y > 0 ? 2 : 3; // +Y, -Y
		else
			return direction.z > 0 ? 4 : 5; // +Z, -Z
	}

	DirectX::XMFLOAT4 sampleCubeMap(const DirectX::ScratchImage& cubeImage, float u, float v, size_t face)
	{
		const DirectX::Image* img = cubeImage.GetImage(0, face, 0);
		if (!img) return { 0, 0, 0, 1 };

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

		auto getPixel = [&](int x, int y) -> DirectX::XMFLOAT4 {
			const float* pixel = reinterpret_cast<const float*>(img->pixels + y * img->rowPitch + x * 4 * sizeof(float));
			return { pixel[0], pixel[1], pixel[2], pixel[3] };
		};

		DirectX::XMFLOAT4 c00 = getPixel(x0, y0);
		DirectX::XMFLOAT4 c10 = getPixel(x1, y0);
		DirectX::XMFLOAT4 c01 = getPixel(x0, y1);
		DirectX::XMFLOAT4 c11 = getPixel(x1, y1);

		DirectX::XMFLOAT4 result;
		result.x = c00.x * (1 - dx) * (1 - dy) + c10.x * dx * (1 - dy) + c01.x * (1 - dx) * dy + c11.x * dx * dy;
		result.y = c00.y * (1 - dx) * (1 - dy) + c10.y * dx * (1 - dy) + c01.y * (1 - dx) * dy + c11.y * dx * dy;
		result.z = c00.z * (1 - dx) * (1 - dy) + c10.z * dx * (1 - dy) + c01.z * (1 - dx) * dy + c11.z * dx * dy;
		result.w = c00.w * (1 - dx) * (1 - dy) + c10.w * dx * (1 - dy) + c01.w * (1 - dx) * dy + c11.w * dx * dy;

		return result;
	}

	DirectX::XMFLOAT4 computeIrradiance(const DirectX::ScratchImage& envCubeImage,
		const DirectX::XMFLOAT3& normal,
		const std::vector<DirectX::XMFLOAT3>& sampleDirections)
	{
		DirectX::XMFLOAT4 irradiance(0, 0, 0, 1);

		DirectX::XMVECTOR N = XMLoadFloat3(&normal);
		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR tangent = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(up, N));
		DirectX::XMVECTOR bitangent = DirectX::XMVector3Cross(N, tangent);

		float weightSum = 0.0f;
		for (const auto& sample : sampleDirections)
		{
			DirectX::XMVECTOR sampleVec = DirectX::XMVectorSet(
				sample.x * tangent.m128_f32[0] + sample.y * bitangent.m128_f32[0] + sample.z * N.m128_f32[0],
				sample.x * tangent.m128_f32[1] + sample.y * bitangent.m128_f32[1] + sample.z * N.m128_f32[1],
				sample.x * tangent.m128_f32[2] + sample.y * bitangent.m128_f32[2] + sample.z * N.m128_f32[2],
				0.0f
			);

			float cosTheta = DirectX::XMVectorGetX(DirectX::XMVector3Dot(sampleVec, N));
			if (cosTheta <= 0.0f) continue;

			DirectX::XMFLOAT3 sampleDir;
			DirectX::XMStoreFloat3(&sampleDir, sampleVec);
			DirectX::XMFLOAT2 uv = directionToCubeUV(sampleDir);

			size_t face = getCubeMapFace(sampleDir);
			DirectX::XMFLOAT4 color = sampleCubeMap(envCubeImage, uv.x, uv.y, face);

			float weight = cosTheta * DirectX::XM_PI;
			irradiance.x += color.x * weight;
			irradiance.y += color.y * weight;
			irradiance.z += color.z * weight;
			weightSum += weight;
		}

		if (weightSum > 0.0f)
		{
			float invWeightSum = 1.0f / weightSum;
			irradiance.x *= invWeightSum;
			irradiance.y *= invWeightSum;
			irradiance.z *= invWeightSum;
		}

		return irradiance;
	}

	void HDRTextureLoader::GenerateIrradianceImage(DirectX::ScratchImage& irradianceImage, const DirectX::ScratchImage& cubeImage, uint32_t irradianceSize, uint32_t sampleCount, uint32_t threadCount)
	{
		DirectX::TexMetadata meta = {};
		meta.format = cubeImage.GetMetadata().format;
		meta.width = irradianceSize;
		meta.height = irradianceSize;
		meta.depth = 1;
		meta.arraySize = 6;
		meta.mipLevels = 1;
		meta.dimension = DirectX::TEX_DIMENSION_TEXTURE2D;
		meta.miscFlags = DirectX::TEX_MISC_TEXTURECUBE;

		irradianceImage.Initialize(meta);

		std::vector<DirectX::XMFLOAT3> sampleDirections = generateSphericalSamples(sampleCount);
		const DirectX::XMFLOAT3 faceDirections[6][3] =
		{

			{ {1,0,0}, {0,1,0}, {0,0,1} },
			{ {-1,0,0},{0,1,0}, {0,0,-1} },
			{ {1,0,0}, {0,0,1}, {0,1,0} },
			{ {1,0,0}, {0,0,-1},{0,-1,0}},
			{ {0,0,1}, {0,1,0}, {-1,0,0} },
			{ {0,0,-1},{0,1,0}, {1,0,0} }
		};


		std::mutex mtx;
		std::vector<std::thread> threads;
		threads.reserve(threadCount);

		auto processFace = [&](uint32_t face)
		{
			const DirectX::Image* destImage = irradianceImage.GetImage(0, face, 0);

			const DirectX::XMFLOAT3& right = faceDirections[face][0];
			const DirectX::XMFLOAT3& up = faceDirections[face][1];
			const DirectX::XMFLOAT3& forward = faceDirections[face][2];

			for (size_t y = 0; y < irradianceSize; y++)
			{
				for (size_t x = 0; x < irradianceSize; x++)
				{
					float u = 2.0f * ((float)x / (irradianceSize - 1)) - 1.0f;
					float v = 1.0f - 2.0f * ((float)y / (irradianceSize - 1));

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

					DirectX::XMFLOAT3 direction(
						right.x * u + up.x * v + forward.x,
						right.y * u + up.y * v + forward.y,
						right.z * u + up.z * v + forward.z
					);

					DirectX::XMVECTOR dirVec = XMLoadFloat3(&direction);
					dirVec = DirectX::XMVector3Normalize(dirVec);
					XMStoreFloat3(&direction, dirVec);

					DirectX::XMFLOAT4 irradiance = computeIrradiance(cubeImage, direction, sampleDirections);

					float* pixel = reinterpret_cast<float*>(destImage->pixels + y * destImage->rowPitch + x * 4 * sizeof(float));
					pixel[0] = irradiance.x;
					pixel[1] = irradiance.y;
					pixel[2] = irradiance.z;
					pixel[3] = irradiance.w;
				}
			}

			std::lock_guard<std::mutex> lock(mtx);
			LOG_INFO("Finished irradiancemap face {0}/6", std::to_string(face));
		};

		for (uint32_t face = 0; face < 6; face++)
		{
			threads.emplace_back(processFace, face);
			//if (threads.size() >= threadCount)
			//{
			//	threads.back().join();
			//	threads.pop_back();
			//}
		}

		for (auto& thread : threads)
			thread.join();

		
		LOG_TRACE("===================End=====================================");
	}

	void HDRTextureLoader::ConvertIrradianceImageToTexture(bool& loadSucceed, ID3D11Resource*& resource, ID3D11ShaderResourceView*& shaderResourceView, const DirectX::ScratchImage& irradianceImage)
	{
		const auto& irradianceMetadata = irradianceImage.GetMetadata();
		CD3D11_TEXTURE2D_DESC textureDesc(irradianceMetadata.format, (unsigned int)irradianceMetadata.width, (unsigned int)irradianceMetadata.height, 6, 1, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT, 0, 1, 0, D3D11_RESOURCE_MISC_TEXTURECUBE);

		std::vector<D3D11_SUBRESOURCE_DATA> initData;
		initData.resize(irradianceMetadata.arraySize * irradianceMetadata.mipLevels);

		for (size_t item = 0; item < irradianceMetadata.arraySize; ++item)
		{
			for (size_t level = 0; level < irradianceMetadata.mipLevels; ++level)
			{
				const DirectX::Image* img = irradianceImage.GetImage(level, item, 0);

				size_t index = item * irradianceMetadata.mipLevels + level;
				initData[index].pSysMem = img->pixels;
				initData[index].SysMemPitch = (unsigned int)img->rowPitch;
				initData[index].SysMemSlicePitch = (unsigned int)img->slicePitch;
			}
		}

		HRESULT hr = 0;
		ID3D11Texture2D* texture2d = nullptr;
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateTexture2D(&textureDesc, initData.data(), &texture2d);
		if (SUCCEEDED(hr))
		{
			CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURECUBE, irradianceMetadata.format, 0, 1);
			hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateShaderResourceView(texture2d, &srvDesc, &shaderResourceView);
			if (SUCCEEDED(hr))
			{
				resource = texture2d;
				loadSucceed = true;
			}
			else
			{
				LOG_ERROR("LoadFromHDR CreateIrradianceShaderResourceView failed");
				loadSucceed = false;
			}
		}
		else
		{
			LOG_ERROR("LoadFromHDR CreateIrradianceTexture2D failed");
			loadSucceed = false;
		}
	}
}