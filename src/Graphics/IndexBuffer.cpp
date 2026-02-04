#include "IndexBuffer.h"
#include "Engine/Blob.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"
#include "RHI/Vulkan/VulkanDevice.h"
#include "RHI/Vulkan/VulkanBuffer.h"
#include <d3d11.h>
#include <thread>

namespace Destiny
{
	IndexBuffer::IndexBuffer(IndexType indexType, std::shared_ptr<Blob> data) :
		m_indexType(indexType),
		m_indexBuffer(nullptr),
		m_data(data)
	{

	}

	IndexBuffer::~IndexBuffer()
	{
		auto engine = Engine::GetInstance();
		if (engine && engine->getGraphicsSystem())
		{
			auto device = engine->getGraphicsSystem()->getDevice();
			if (device && std::dynamic_pointer_cast<VulkanDevice>(device))
			{
				if (m_indexBuffer) delete (VulkanBuffer*)m_indexBuffer;
			}
			else
			{
				if (m_indexBuffer) ((ID3D11Buffer*)m_indexBuffer)->Release();
			}
		}
		// m_indexBuffer = nullptr;
	}

	void IndexBuffer::doLoad()
	{
		if (!m_data)
		{
			loadFailed__();
			LOG_ERROR("Thread {0}, IndexBuffer load failed", std::to_string((*(uint32_t*)&std::this_thread::get_id())));
			return;
		}

		// Cleanup
		auto device = Engine::GetInstance()->getGraphicsSystem()->getDevice();
		if (std::dynamic_pointer_cast<VulkanDevice>(device))
		{
			if (m_indexBuffer) delete (VulkanBuffer*)m_indexBuffer;
		}
		else
		{
			if (m_indexBuffer) ((ID3D11Buffer*)m_indexBuffer)->Release();
		}
		m_indexBuffer = nullptr;
		
		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = (UINT)m_data->getLength();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = m_data->getData();

		
		HRESULT hr = (HRESULT)device->CreateBuffer(&ibd, &InitData, (void**)&m_indexBuffer);
		
		if (SUCCEEDED(hr))
		{
			loadSucceeded__();
			m_data.reset();
		}
		else
		{
			loadFailed__();
			LOG_ERROR("Thread {0}, IndexBuffer load failed", std::to_string((*(uint32_t*)&std::this_thread::get_id())));
			m_data.reset();
		}
	}
}