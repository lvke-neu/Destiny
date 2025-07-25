#include "IndirectBuffer.h"
#include "Engine/Blob.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny
{
	IndirectBuffer::IndirectBuffer(std::shared_ptr<Blob> data) :
		m_indirectBuffer(nullptr),
		m_data(data)
	{

	}

	IndirectBuffer::~IndirectBuffer()
	{
		SAFE_RELEASE(m_indirectBuffer);
	}

	void IndirectBuffer::doLoad()
	{
		if (!m_data)
		{
			loadFailed__();
			LOG_ERROR("Thread {0}, IndirectBuffer load failed", std::to_string((*(uint32_t*)&std::this_thread::get_id())));
			return;
		}

		SAFE_RELEASE(m_indirectBuffer);

		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));
		ibd.ByteWidth = (UINT)m_data->getLength();
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0;
		ibd.BindFlags = 0;
		ibd.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory(&initData, sizeof(initData));
		initData.pSysMem = m_data->getData();

		
		HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBuffer(&ibd, &initData, &m_indirectBuffer);
		
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