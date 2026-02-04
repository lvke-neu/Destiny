#include "VertexBuffer.h"
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
	VertexBuffer::VertexBuffer(std::shared_ptr<InputLayout> inputLayout, unsigned int stride, unsigned int offset, std::shared_ptr<Blob> data) :
		m_inputLayout(inputLayout),
		m_stride(stride),
		m_offset(offset),
		m_vertexBuffer(nullptr),
		m_data(data)
	{

	}

	VertexBuffer::~VertexBuffer()
	{
		auto engine = Engine::GetInstance();
		if (engine && engine->getGraphicsSystem())
		{
			auto device = engine->getGraphicsSystem()->getDevice();
			if (device && std::dynamic_pointer_cast<VulkanDevice>(device))
			{
				if (m_vertexBuffer) delete (VulkanBuffer*)m_vertexBuffer;
			}
			else
			{
				if (m_vertexBuffer) ((ID3D11Buffer*)m_vertexBuffer)->Release();
			}
		}
		// m_vertexBuffer = nullptr; // Not strictly needed in destructor
	}

	void VertexBuffer::doLoad()
	{
		if (!m_inputLayout || !m_data)
		{
			loadFailed__();
			LOG_ERROR("Thread {0}, VertexBuffer load failed", std::to_string((*(uint32_t*)&std::this_thread::get_id())));
			return;
		}

		// Cleanup existing buffer
		auto device = Engine::GetInstance()->getGraphicsSystem()->getDevice();
		if (std::dynamic_pointer_cast<VulkanDevice>(device))
		{
			if (m_vertexBuffer) delete (VulkanBuffer*)m_vertexBuffer;
		}
		else
		{
			if (m_vertexBuffer) ((ID3D11Buffer*)m_vertexBuffer)->Release();
		}
		m_vertexBuffer = nullptr;

		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));
		ibd.Usage = D3D11_USAGE_DYNAMIC;
		ibd.ByteWidth = (UINT)m_data->getLength();
		ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = m_data->getData();
		
		HRESULT hr = (HRESULT)device->CreateBuffer(&ibd, &InitData, (void**)&m_vertexBuffer);
		
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

	void VertexBuffer::modify(std::shared_ptr<Blob> data)
	{
		if (!data)
		{
			return;
		}
		m_data = data;
		doLoad();
	}
}