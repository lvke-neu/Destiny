#include "VertexShader.h"
#include "Engine/Utility.h"
#include <d3dcompiler.h>
#include <d3d11.h>

namespace Destiny
{

	VertexShader::VertexShader(ID3D11Device* device, const wchar_t* fileName, const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, unsigned int NumElements)
	{
		ID3DBlob* blob{ nullptr };
		D3DReadFileToBlob(fileName, &blob);
		device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), 0, &m_pVertexShader);
		device->CreateInputLayout(pInputElementDescs, NumElements, blob->GetBufferPointer(), blob->GetBufferSize(), &m_pInputLayout);
		SAFE_RELEASE(blob);
	}

	VertexShader::~VertexShader()
	{
		SAFE_RELEASE(m_pInputLayout);
		SAFE_RELEASE(m_pVertexShader);
	}

}