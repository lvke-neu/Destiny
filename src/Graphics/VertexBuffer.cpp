#include "VertexBuffer.h"
#include "Engine/Utility.h"
#include <d3d11.h>

namespace Destiny
{

	VertexBuffer::VertexBuffer(ID3D11Device* device, unsigned int stride, unsigned int offset, const void* vertexData, unsigned int vertexDataSize) :
		m_stride(stride), m_offset(offset)
	{
		D3D11_BUFFER_DESC vbd;
		ZeroMemory(&vbd, sizeof(vbd));
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = vertexDataSize;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertexData;

		device->CreateBuffer(&vbd, &InitData, &m_pVertexBuffer);
	}

	VertexBuffer::~VertexBuffer()
	{
		SAFE_RELEASE(m_pVertexBuffer);
	}
}