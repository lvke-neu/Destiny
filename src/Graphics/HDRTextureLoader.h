#pragma once
#include "ThirdParty/DirectXTex/DirectXTex.h"
#include <thread>

namespace Destiny
{
	class HDRTextureLoader
	{
	public:
		static void GenerateCubeImage(DirectX::ScratchImage& cubeImage, const DirectX::ScratchImage& srcImage, uint32_t threadCount = std::thread::hardware_concurrency());
		static void ConvertCubeImageToTexture(bool& loadSucceed, void*& resource, void*& shaderResourceView, const DirectX::ScratchImage& cubeImage);
		
		static void GenerateIrradianceImage(DirectX::ScratchImage& irradianceImage, const DirectX::ScratchImage& cubeImage, uint32_t irradianceSize = 32, uint32_t sampleCount = 1024, uint32_t threadCount = std::thread::hardware_concurrency());
		static void ConvertIrradianceImageToTexture(bool& loadSucceed, void*& resource, void*& shaderResourceView, const DirectX::ScratchImage& irradianceImage);
		
		static void GeneratePrefilterMap(DirectX::ScratchImage& prefilterImage, const DirectX::ScratchImage& cubeImage, uint32_t threadCount = std::thread::hardware_concurrency());
		static void ConvertPrefilterImageToTexture(bool& loadSucceed, void*& resource, void*& shaderResourceView, const DirectX::ScratchImage& prefilterImage);
	
		static void GenerateBRDFLUT(uint32_t width = 512, uint32_t height = 512);
	};
}