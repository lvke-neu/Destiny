#include "IndexBuffer.h"
#include "Engine/Utility.h"
#include <d3d11.h>

namespace Destiny
{

	IndexBuffer::IndexBuffer(ID3D11Device* device, DXGI_FORMAT format, const void* indexData, unsigned int indexDataSize) : 
		m_format(format)
	{
		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = indexDataSize;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = indexData;

		device->CreateBuffer(&ibd, &InitData, &m_pIndexBuffer);
	}

	IndexBuffer::~IndexBuffer()
	{
		SAFE_RELEASE(m_pIndexBuffer);
	}
}