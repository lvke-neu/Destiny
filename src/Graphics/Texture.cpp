#include "Texture.h"
#include "Engine/Utility.h"
#include "DDSTextureLoader.h"
#include <d3d11.h>

namespace Destiny
{

	Texture::Texture(ID3D11Device* device, const wchar_t* fileName)
	{
		DirectX::CreateDDSTextureFromFile(device, fileName, nullptr, &m_pShaderResourceView);
	}

	Texture::~Texture()
	{
		SAFE_RELEASE(m_pShaderResourceView);
	}
}