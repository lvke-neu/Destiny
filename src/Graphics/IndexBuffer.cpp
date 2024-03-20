#include "IndexBuffer.h"
#include "Engine/Blob.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny
{
	IndexBuffer::IndexBuffer(IndexType indexType, std::shared_ptr<Blob> data) :
		m_indexType(indexType),
		m_data(data)
	{

	}

	IndexBuffer::~IndexBuffer()
	{
		SAFE_RELEASE(m_indexBuffer);
	}

	void IndexBuffer::doLoad()
	{
		if (!m_data)
		{
			loadFailed__();
			LOG_ERROR("Thread {0}, IndexBuffer load failed : {1}", std::to_string((*(uint32_t*)&std::this_thread::get_id())));
			return;
		}
		
		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = (UINT)m_data->getLength();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = m_data->getData();

		HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBuffer(&ibd, &InitData, &m_indexBuffer);

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