#pragma once
#include "ThirdParty/DirectXTex/DirectXTex.h"
#include <thread>

namespace Destiny
{
	class HDRTextureLoader
	{
	public:
		static void GenerateCubeImage(DirectX::ScratchImage& cubeImage, const DirectX::ScratchImage& srcImage, uint32_t threadCount = std::thread::hardware_concurrency());
		static void ConvertCubeImageToTexture(bool& loadSucceed, ID3D11Resource*& resource, ID3D11ShaderResourceView*& shaderResourceView, const DirectX::ScratchImage& cubeImage);
	};
}