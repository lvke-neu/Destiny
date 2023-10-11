#include "PixelShader.h"
#include "Engine/Utility.h"
#include <d3dcompiler.h>
#include <d3d11.h>

namespace Destiny
{

	PixelShader::PixelShader(ID3D11Device* device, const wchar_t* fileName)
	{
		ID3DBlob* blob{ nullptr };
		D3DReadFileToBlob(fileName, &blob);
		device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pPixelShader);
		SAFE_RELEASE(blob);
	}

	PixelShader::~PixelShader()
	{
		SAFE_RELEASE(m_pPixelShader);
	}

}