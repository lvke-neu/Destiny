#include "VertexBuffer.h"
#include "Engine/Blob.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"
#include <d3d11.h>

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
		SAFE_RELEASE(m_vertexBuffer);
	}

	void VertexBuffer::doLoad()
	{
		if (!m_inputLayout || !m_data)
		{
			loadFailed__();
			LOG_ERROR("Thread {0}, VertexBuffer load failed", std::to_string((*(uint32_t*)&std::this_thread::get_id())));
			return;
		}

		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = (UINT)m_data->getLength();
		ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		ibd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = m_data->getData();

		HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBuffer(&ibd, &InitData, &m_vertexBuffer);

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