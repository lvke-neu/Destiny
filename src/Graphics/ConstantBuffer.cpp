#include "ConstantBuffer.h"
#include "Engine/Utility.h"
#include <d3d11.h>

namespace Destiny
{
	ConstantBuffer::ConstantBuffer(ID3D11Device* device, unsigned int dataSize)
	{
		D3D11_BUFFER_DESC cbd;
		ZeroMemory(&cbd, sizeof(cbd));
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.ByteWidth = dataSize;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		device->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
	}

	ConstantBuffer::~ConstantBuffer()
	{
		SAFE_RELEASE(m_pConstantBuffer);
	}

	void ConstantBuffer::updateData(ID3D11DeviceContext* context, const void* data, unsigned int dataSize)
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		context->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		memcpy_s(mappedData.pData, dataSize, data, dataSize);
		context->Unmap(m_pConstantBuffer, 0);
	}
}