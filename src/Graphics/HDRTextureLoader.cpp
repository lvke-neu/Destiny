#include "HDRTextureLoader.h"
#include "Engine/Utility.h"
#include "Graphics/GraphicsSystem.h"
#include <mutex>
#include <random>
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
			if (threads.size() >= threadCount)
			{
				threads.back().join();
				threads.pop_back();
			}
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
			if (threads.size() >= threadCount)
			{
				threads.back().join();
				threads.pop_back();
			}
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

	using namespace DirectX;

    struct alignas(16) ImportanceSampleSIMD
    {
        XMVECTOR direction;
        float pdf;         
        uint32_t pad[3];
    };

 
    std::vector<std::vector<ImportanceSampleSIMD>> precomputedSamplesSIMD;
    std::once_flag samplesInitializedSIMD;


    void InitializeImportanceSamplesSIMD(int maxSamples = 1024)
    {
        precomputedSamplesSIMD.resize(64);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0f, 1.0f);

        for (int roughnessLevel = 0; roughnessLevel < 64; roughnessLevel++)
        {
            float roughness = static_cast<float>(roughnessLevel) / 63.0f;
            int numSamples = std::max(16, static_cast<int>(maxSamples * (1.0f - roughness) + 1));


            numSamples = (numSamples + 3) & ~3;

            precomputedSamplesSIMD[roughnessLevel].resize(numSamples);

            for (int i = 0; i < numSamples; i++)
            {
 
                float a = dis(gen);
                float b = dis(gen);


                float phi = 2.0f * XM_PI * a;
                float cosTheta = sqrtf((1.0f - b) / (1.0f + (roughness * roughness - 1.0f) * b));
                float sinTheta = sqrtf(1.0f - cosTheta * cosTheta);

                ImportanceSampleSIMD sample;
                sample.direction = XMVectorSet(
                    sinTheta * cosf(phi),
                    sinTheta * sinf(phi),
                    cosTheta,
                    0.0f
                );


                float a2 = roughness * roughness;
                float d = (cosTheta * a2 - cosTheta) * cosTheta + 1;
                sample.pdf = (a2 / (XM_PI * d * d)) * (cosTheta / sinTheta);

                precomputedSamplesSIMD[roughnessLevel][i] = sample;
            }
        }
    }

 
    __forceinline XMVECTOR DistributionGGX_SIMD(XMVECTOR NdotH, float roughness)
    {
        float a = roughness * roughness;
        float a2 = a * a;

        // d = (NdotH * a2 - NdotH) * NdotH + 1
        XMVECTOR d = XMVectorMultiplyAdd(
            XMVectorSubtract(
                XMVectorMultiply(NdotH, XMVectorReplicate(a2)),
                NdotH
            ),
            NdotH,
            XMVectorReplicate(1.0f)
        );

        // return a2 / (XM_PI * d * d)
        return XMVectorDivide(
            XMVectorReplicate(a2),
            XMVectorMultiply(
                XMVectorReplicate(XM_PI),
                XMVectorMultiply(d, d)
            )
        );
    }



	XMFLOAT2 CubeMapDirectionToUV(const XMFLOAT3& direction, uint32_t face)
	{
		XMFLOAT2 uv = { 0.0f, 0.0f };
		float x = direction.x;
		float y = direction.y;
		float z = direction.z;

		switch (face)
		{
		case 0: 
			uv.x = -z;
			uv.y = y;
			break;
		case 1: 
			uv.x = z;
			uv.y = y;
			break;
		case 2: 
			uv.x = x;
			uv.y = z;
			break;
		case 3: 
			uv.x = x;
			uv.y = -z;
			break;
		case 4: 
			uv.x = x;
			uv.y = y;
			break;
		case 5: 
			uv.x = -x;
			uv.y = y;
			break;
		}


		uv.x = clamp(uv.x, -1.0f, 1.0f) * 0.5f + 0.5f; 
		uv.y = clamp(uv.y, -1.0f, 1.0f) * 0.5f + 0.5f;
		return uv;
	}


	XMFLOAT4 SampleCubeMap(const DirectX::ScratchImage& cubeImage, float u, float v, uint32_t face, uint32_t mipLevel)
	{
		const DirectX::Image* img = cubeImage.GetImage(mipLevel, face, 0);
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

    void HDRTextureLoader::GeneratePrefilterMap(DirectX::ScratchImage& prefilterImage, const DirectX::ScratchImage& cubeImage, uint32_t threadCount)
    {
        const TexMetadata& srcMeta = cubeImage.GetMetadata();
        size_t baseSize = srcMeta.width;


        uint32_t mipCount = 1 + static_cast<uint32_t>(log2f(static_cast<float>(baseSize)));

 
        TexMetadata meta = {};
        meta.format = srcMeta.format;
        meta.width = baseSize;
        meta.height = baseSize;
        meta.depth = 1;
        meta.arraySize = 6;
        meta.mipLevels = mipCount;
        meta.dimension = TEX_DIMENSION_TEXTURE2D;
        meta.miscFlags = TEX_MISC_TEXTURECUBE;


		prefilterImage.Initialize(meta);


        std::call_once(samplesInitializedSIMD, []() {
            InitializeImportanceSamplesSIMD(1024);
            });


        struct WorkItem
        {
            uint32_t mipLevel;
            uint32_t face;
        };

        std::vector<WorkItem> workItems;
        for (uint32_t mipLevel = 0; mipLevel < mipCount; mipLevel++)
        {
            for (uint32_t face = 0; face < 6; face++)
            {
                workItems.push_back({ mipLevel, face });
            }
        }

        std::atomic<size_t> workIndex(0);
        std::mutex logMutex;
        std::atomic<uint32_t> completedJobs(0);
        const uint32_t totalJobs = static_cast<uint32_t>(workItems.size());

        std::vector<std::thread> threads;
        threads.reserve(threadCount);


        const XMVECTOR faceDirections[6][3] =
        {
            { XMVectorSet(1,0,0,0), XMVectorSet(0,1,0,0), XMVectorSet(0,0,1,0) },
            { XMVectorSet(-1,0,0,0), XMVectorSet(0,1,0,0), XMVectorSet(0,0,-1,0) },
            { XMVectorSet(0,1,0,0), XMVectorSet(0,0,1,0), XMVectorSet(1,0,0,0) },
            { XMVectorSet(0,1,0,0), XMVectorSet(0,0,-1,0), XMVectorSet(-1,0,0,0) },
            { XMVectorSet(0,0,1,0), XMVectorSet(1,0,0,0), XMVectorSet(0,1,0,0) },
            { XMVectorSet(0,0,-1,0), XMVectorSet(1,0,0,0), XMVectorSet(0,-1,0,0) }
        };

        const XMVECTOR VEC_ZERO = XMVectorZero();
        const XMVECTOR VEC_ONE = XMVectorReplicate(1.0f);
        const XMVECTOR VEC_TWO = XMVectorReplicate(2.0f);

        auto workerFunction = [&]()
        {
            while (true)
            {
                size_t index = workIndex.fetch_add(1);
                if (index >= workItems.size())
                    break;

                const WorkItem& work = workItems[index];
                uint32_t mipLevel = work.mipLevel;
                uint32_t face = work.face;

                float roughness = static_cast<float>(mipLevel) / static_cast<float>(mipCount - 1);
                int roughnessLevel = static_cast<int>(roughness * 63.0f);
                roughnessLevel = clamp(roughnessLevel, 0, 63);


                size_t mipSize = std::max((size_t)1u, static_cast<size_t>(baseSize >> mipLevel));

                const Image* destImage = prefilterImage.GetImage(mipLevel, face, 0);
                const XMVECTOR& right = faceDirections[face][0];
                const XMVECTOR& up = faceDirections[face][1];
                const XMVECTOR& forward = faceDirections[face][2];

                const std::vector<ImportanceSampleSIMD>& samples = precomputedSamplesSIMD[roughnessLevel];
                int numSamples = static_cast<int>(samples.size());

                for (size_t y = 0; y < mipSize; y++)
                {
                    for (size_t x = 0; x < mipSize; x++)
                    {

                        float u = 2.0f * ((float)x / (mipSize - 1)) - 1.0f;
                        float v = 1.0f - 2.0f * ((float)y / (mipSize - 1));

                        XMVECTOR direction = XMVectorAdd(
                            XMVectorAdd(
                                XMVectorScale(right, u),
                                XMVectorScale(up, v)
                            ),
                            forward
                        );

                        direction = XMVector3Normalize(direction);


                        XMVECTOR N = direction;

                        XMVECTOR tangent = XMVector3Normalize(XMVectorSet(1, 0, 0, 0));
                        XMVECTOR bitangent = XMVector3Cross(N, tangent);
                        tangent = XMVector3Cross(bitangent, N);


                        XMMATRIX TBN = XMMatrixSet(
                            XMVectorGetX(tangent), XMVectorGetY(tangent), XMVectorGetZ(tangent), 0,
                            XMVectorGetX(bitangent), XMVectorGetY(bitangent), XMVectorGetZ(bitangent), 0,
                            XMVectorGetX(N), XMVectorGetY(N), XMVectorGetZ(N), 0,
                            0, 0, 0, 1
                        );


                        XMVECTOR accumulatorRGB = XMVectorZero();
                        XMVECTOR accumulatorWeight = XMVectorZero();

                        for (int i = 0; i < numSamples; i += 4)
                        {

                            XMVECTOR H0 = samples[i + 0].direction;
                            XMVECTOR H1 = samples[i + 1].direction;
                            XMVECTOR H2 = samples[i + 2].direction;
                            XMVECTOR H3 = samples[i + 3].direction;


                            H0 = XMVector3TransformNormal(H0, TBN);
                            H1 = XMVector3TransformNormal(H1, TBN);
                            H2 = XMVector3TransformNormal(H2, TBN);
                            H3 = XMVector3TransformNormal(H3, TBN);

                            H0 = XMVector3Normalize(H0);
                            H1 = XMVector3Normalize(H1);
                            H2 = XMVector3Normalize(H2);
                            H3 = XMVector3Normalize(H3);

                            XMVECTOR NdotH0 = XMVector3Dot(N, H0);
                            XMVECTOR NdotH1 = XMVector3Dot(N, H1);
                            XMVECTOR NdotH2 = XMVector3Dot(N, H2);
                            XMVECTOR NdotH3 = XMVector3Dot(N, H3);

                            XMVECTOR R0 = XMVectorSubtract(
                                XMVectorScale(H0, XMVectorGetX(XMVectorMultiply(NdotH0, VEC_TWO))),
                                direction
                            );

                            XMVECTOR R1 = XMVectorSubtract(
                                XMVectorScale(H1, XMVectorGetX(XMVectorMultiply(NdotH1, VEC_TWO))),
                                direction
                            );

                            XMVECTOR R2 = XMVectorSubtract(
                                XMVectorScale(H2, XMVectorGetX(XMVectorMultiply(NdotH2, VEC_TWO))),
                                direction
                            );

                            XMVECTOR R3 = XMVectorSubtract(
                                XMVectorScale(H3, XMVectorGetX(XMVectorMultiply(NdotH3, VEC_TWO))),
                                direction
                            );


                            R0 = XMVector3Normalize(R0);
                            R1 = XMVector3Normalize(R1);
                            R2 = XMVector3Normalize(R2);
                            R3 = XMVector3Normalize(R3);


                            XMVECTOR NdotL0 = XMVector3Dot(N, R0);
                            XMVECTOR NdotL1 = XMVector3Dot(N, R1);
                            XMVECTOR NdotL2 = XMVector3Dot(N, R2);
                            XMVECTOR NdotL3 = XMVector3Dot(N, R3);

            
                            XMVECTOR D0 = DistributionGGX_SIMD(NdotH0, roughness);
                            XMVECTOR D1 = DistributionGGX_SIMD(NdotH1, roughness);
                            XMVECTOR D2 = DistributionGGX_SIMD(NdotH2, roughness);
                            XMVECTOR D3 = DistributionGGX_SIMD(NdotH3, roughness);


                            XMVECTOR weight0 = XMVectorDivide(
                                XMVectorMultiply(D0, NdotH0),
                                XMVectorAdd(
                                    XMVectorMultiply(NdotH0, XMVectorReplicate(4.0f)),
                                    XMVectorReplicate(0.0001f)
                                )
                            );

                            XMVECTOR weight1 = XMVectorDivide(
                                XMVectorMultiply(D1, NdotH1),
                                XMVectorAdd(
                                    XMVectorMultiply(NdotH1, XMVectorReplicate(4.0f)),
                                    XMVectorReplicate(0.0001f)
                                )
                            );

                            XMVECTOR weight2 = XMVectorDivide(
                                XMVectorMultiply(D2, NdotH2),
                                XMVectorAdd(
                                    XMVectorMultiply(NdotH2, XMVectorReplicate(4.0f)),
                                    XMVectorReplicate(0.0001f)
                                )
                            );

                            XMVECTOR weight3 = XMVectorDivide(
                                XMVectorMultiply(D3, NdotH3),
                                XMVectorAdd(
                                    XMVectorMultiply(NdotH3, XMVectorReplicate(4.0f)),
                                    XMVectorReplicate(0.0001f)
                                )
                            );


                            bool valid0 = XMVectorGetX(NdotL0) > 0.0f;
                            bool valid1 = XMVectorGetX(NdotL1) > 0.0f;
                            bool valid2 = XMVectorGetX(NdotL2) > 0.0f;
                            bool valid3 = XMVectorGetX(NdotL3) > 0.0f;

 
                            if (valid0)
                            {
                                XMFLOAT3 reflection;
                                XMStoreFloat3(&reflection, R0);
          
								XMFLOAT2 uv = CubeMapDirectionToUV(reflection, face);

								float roughnessMip = roughness * (mipCount - 1);
								uint32_t sampleMip = static_cast<uint32_t>(floor(roughnessMip));
								sampleMip = std::min(sampleMip, (uint32_t)cubeImage.GetMetadata().mipLevels - 1);

								XMFLOAT4 sampleColor = SampleCubeMap(cubeImage, uv.x, uv.y, face, sampleMip);

                                XMVECTOR color = XMVectorSet(
                                    sampleColor.x,
                                    sampleColor.y,
                                    sampleColor.z,
                                    0.0f
                                );

                                float w = XMVectorGetX(weight0) * XMVectorGetX(NdotL0);
                                accumulatorRGB = XMVectorAdd(
                                    accumulatorRGB,
                                    XMVectorScale(color, w)
                                );
                                accumulatorWeight = XMVectorAdd(
                                    accumulatorWeight,
                                    XMVectorReplicate(w)
                                );
                            }

                            if (valid1)
                            {
                                XMFLOAT3 reflection;
                                XMStoreFloat3(&reflection, R1);
              
								XMFLOAT2 uv = CubeMapDirectionToUV(reflection, face);

								float roughnessMip = roughness * (mipCount - 1);
								uint32_t sampleMip = static_cast<uint32_t>(floor(roughnessMip));
								sampleMip = std::min(sampleMip, (uint32_t)cubeImage.GetMetadata().mipLevels - 1);

								XMFLOAT4 sampleColor = SampleCubeMap(cubeImage, uv.x, uv.y, face, sampleMip);

                                XMVECTOR color = XMVectorSet(
                                    sampleColor.x,
                                    sampleColor.y,
                                    sampleColor.z,
                                    0.0f
                                );

                                float w = XMVectorGetX(weight1) * XMVectorGetX(NdotL1);
                                accumulatorRGB = XMVectorAdd(
                                    accumulatorRGB,
                                    XMVectorScale(color, w)
                                );
                                accumulatorWeight = XMVectorAdd(
                                    accumulatorWeight,
                                    XMVectorReplicate(w)
                                );
                            }

                            if (valid2)
                            {
                                XMFLOAT3 reflection;
                                XMStoreFloat3(&reflection, R2);

								XMFLOAT2 uv = CubeMapDirectionToUV(reflection, face);

								float roughnessMip = roughness * (mipCount - 1);
								uint32_t sampleMip = static_cast<uint32_t>(floor(roughnessMip));
								sampleMip = std::min(sampleMip, (uint32_t)cubeImage.GetMetadata().mipLevels - 1);

								XMFLOAT4 sampleColor = SampleCubeMap(cubeImage, uv.x, uv.y, face, sampleMip);

                                XMVECTOR color = XMVectorSet(
                                    sampleColor.x,
                                    sampleColor.y,
                                    sampleColor.z,
                                    0.0f
                                );

                                float w = XMVectorGetX(weight2) * XMVectorGetX(NdotL2);
                                accumulatorRGB = XMVectorAdd(
                                    accumulatorRGB,
                                    XMVectorScale(color, w)
                                );
                                accumulatorWeight = XMVectorAdd(
                                    accumulatorWeight,
                                    XMVectorReplicate(w)
                                );
                            }

                            if (valid3)
                            {
                                XMFLOAT3 reflection;
                                XMStoreFloat3(&reflection, R3);

								XMFLOAT2 uv = CubeMapDirectionToUV(reflection, face);

								float roughnessMip = roughness * (mipCount - 1);
								uint32_t sampleMip = static_cast<uint32_t>(floor(roughnessMip));
								sampleMip = std::min(sampleMip, (uint32_t)cubeImage.GetMetadata().mipLevels - 1);

								XMFLOAT4 sampleColor = SampleCubeMap(cubeImage, uv.x, uv.y, face, sampleMip);

                                XMVECTOR color = XMVectorSet(
                                    sampleColor.x,
                                    sampleColor.y,
                                    sampleColor.z,
                                    0.0f
                                );

                                float w = XMVectorGetX(weight3) * XMVectorGetX(NdotL3);
                                accumulatorRGB = XMVectorAdd(
                                    accumulatorRGB,
                                    XMVectorScale(color, w)
                                );
                                accumulatorWeight = XMVectorAdd(
                                    accumulatorWeight,
                                    XMVectorReplicate(w)
                                );
                            }
                        }

 
                        float totalWeight = XMVectorGetX(accumulatorWeight);
                        XMVECTOR result = XMVectorZero();

                        if (totalWeight > 0.0f)
                        {
                            result = XMVectorDivide(accumulatorRGB, XMVectorReplicate(totalWeight));
                        }

     
                        float* pixel = reinterpret_cast<float*>(destImage->pixels + y * destImage->rowPitch + x * 4 * sizeof(float));
                        XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(pixel), XMVectorSetW(result, 1.0f));
                    }
                }


                uint32_t completed = completedJobs.fetch_add(1) + 1;
                if (completed % 10 == 0 || completed == totalJobs)
                {
                    std::lock_guard<std::mutex> lock(logMutex);
                    LOG_INFO("Prefilter progress: {0}/{1} ({2:.1f}%)",
                        std::to_string(completed),
                        std::to_string(totalJobs),
                        (float)completed / totalJobs * 100.0f);
                }
            }
        };

        for (uint32_t i = 0; i < threadCount; i++)
        {
            threads.emplace_back(workerFunction);
        }

        for (auto& thread : threads)
        {
            thread.join();
        }

        LOG_TRACE("Finished generating prefilter map");
    }

	void HDRTextureLoader::ConvertPrefilterImageToTexture(bool& loadSucceed, ID3D11Resource*& resource, ID3D11ShaderResourceView*& shaderResourceView, const DirectX::ScratchImage& prefilterImage)
	{
		const auto& prefilterMetadata = prefilterImage.GetMetadata();
		CD3D11_TEXTURE2D_DESC textureDesc(prefilterMetadata.format, (unsigned int)prefilterMetadata.width, (unsigned int)prefilterMetadata.height, 6, 1, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT, 0, 1, 0, D3D11_RESOURCE_MISC_TEXTURECUBE);

		std::vector<D3D11_SUBRESOURCE_DATA> initData;
		initData.resize(prefilterMetadata.arraySize * prefilterMetadata.mipLevels);

		for (size_t item = 0; item < prefilterMetadata.arraySize; ++item)
		{
			for (size_t level = 0; level < prefilterMetadata.mipLevels; ++level)
			{
				const DirectX::Image* img = prefilterImage.GetImage(level, item, 0);

				size_t index = item * prefilterMetadata.mipLevels + level;
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
			CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURECUBE, prefilterMetadata.format, 0, 1);
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


	float DistributionGGX(float NdotH, float roughness) {
		float a = roughness * roughness;
		float a2 = a * a;
		float denom = NdotH * NdotH * (a2 - 1.0f) + 1.0f;
		return a2 / (XM_PI * denom * denom);
	}


	float GeometrySchlickGGX(float NdotV, float roughness) {
		float r = (roughness + 1.0f);
		float k = (r * r) / 8.0f;
		return NdotV / (NdotV * (1.0f - k) + k);
	}

	float GeometrySmith(float NdotV, float NdotL, float roughness) {
		float ggx2 = GeometrySchlickGGX(NdotV, roughness);
		float ggx1 = GeometrySchlickGGX(NdotL, roughness);
		return ggx1 * ggx2;
	}
	
	void HDRTextureLoader::GenerateBRDFLUT(uint32_t width, uint32_t height)
	{
		DirectX::ScratchImage lutImage;

		TexMetadata meta = {};
		meta.format = DXGI_FORMAT_R16G16_FLOAT;
		meta.width = width;
		meta.height = height;
		meta.depth = 1;
		meta.arraySize = 1;
		meta.mipLevels = 1;
		meta.dimension = TEX_DIMENSION_TEXTURE2D;

		lutImage.Initialize(meta);
		const Image* img = lutImage.GetImage(0, 0, 0);

		for (uint32_t y = 0; y < height; ++y) {
			for (uint32_t x = 0; x < width; ++x) {

				float NdotV = (float)x / (width - 1);
				float roughness = (float)y / (height - 1);

				XMFLOAT3 V = { sqrtf(1.0f - NdotV * NdotV), 0.0f, NdotV };
				XMFLOAT3 N = { 0.0f, 0.0f, 1.0f };


				const uint32_t sampleCount = 1024;
				float A = 0.0f;
				float B = 0.0f;

				for (uint32_t i = 0; i < sampleCount; ++i) {

					float xi1 = (float)i / sampleCount;
					float xi2 = rand() / (float)RAND_MAX;


					float a = roughness * roughness;
					float phi = 2.0f * XM_PI * xi1;
					float cosTheta = sqrtf((1.0f - xi2) / (1.0f + (a * a - 1.0f) * xi2));
					float sinTheta = sqrtf(1.0f - cosTheta * cosTheta);

					XMFLOAT3 H = {
						sinTheta * cosf(phi),
						sinTheta * sinf(phi),
						cosTheta
					};


					XMFLOAT3 L = {
						2.0f * (V.x * H.x + V.y * H.y + V.z * H.z) * H.x - V.x,
						2.0f * (V.x * H.x + V.y * H.y + V.z * H.z) * H.y - V.y,
						2.0f * (V.x * H.x + V.y * H.y + V.z * H.z) * H.z - V.z
					};


					float NdotL = std::max(L.z, 0.0f);
					float NdotH = std::max(H.z, 0.0f);
					float VdotH = std::max(V.x * H.x + V.y * H.y + V.z * H.z, 0.0f);

					if (NdotL > 0.0f) {
	
						float D = DistributionGGX(NdotH, roughness);
						float G = GeometrySmith(NdotV, NdotL, roughness);
						float Fc = powf(1.0f - VdotH, 5.0f);

	
						float G_Vis = G * VdotH / (NdotH * NdotV);


						float Fc_vis = G_Vis * Fc;
						float Vis = G_Vis * (1.0f - Fc);

						A += Vis;
						B += Fc_vis;
					}
				}


				A /= sampleCount;
				B /= sampleCount;


				float* pixel = reinterpret_cast<float*>(img->pixels + y * img->rowPitch + x * 2 * sizeof(float));
				pixel[0] = A;
				pixel[1] = B;
			}
		}

		DirectX::SaveToDDSFile(
			lutImage.GetImages(),
			lutImage.GetImageCount(),
			lutImage.GetMetadata(),
			DirectX::DDS_FLAGS_NONE,
			Utility::MultiByte2WideChar("D:/C++Project/Destiny/build/Release/builtin/texture/skybox/hdr/BrdfLUT.dds").c_str()
		);
	}
}