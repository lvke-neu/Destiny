#include "InstanceBuffer.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Graphics/GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny
{
	InstanceBuffer::InstanceBuffer(unsigned int byteWidth, unsigned int stride, unsigned int offset, unsigned int instanceCount) :
		m_instanceBuffer(nullptr),
		m_byteWidth(byteWidth),
		m_stride(stride),
		m_offset(offset),
		m_instanceCount(instanceCount)
	{
		D3D11_BUFFER_DESC cbd;
		ZeroMemory(&cbd, sizeof(cbd));
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.ByteWidth = byteWidth;
		cbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		
		HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBuffer(&cbd, nullptr, &m_instanceBuffer);
		if (FAILED(hr))
		{
			
			LOG_ERROR("Create InstanceBuffer Failed");
			return;
		}
		
	}

	InstanceBuffer::~InstanceBuffer()
	{
		SAFE_RELEASE(m_instanceBuffer);
	}

	void InstanceBuffer::updateInstanceData(std::shared_ptr<Blob> data)
	{
		if (!data || m_offset + data->getLength() > m_byteWidth)
		{
			return;
		}

		D3D11_MAPPED_SUBRESOURCE mappedData;
		HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		if (SUCCEEDED(hr))
		{
			memcpy_s(mappedData.pData, data->getLength(), data->getData(), data->getLength());
			Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->Unmap(m_instanceBuffer, 0);
		}
	}
}