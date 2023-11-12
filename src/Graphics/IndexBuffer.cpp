#include "IndexBuffer.h"
#include "Engine/Blob.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"

namespace Destiny
{
	IndexBuffer::IndexBuffer(DXGI_FORMAT format, std::shared_ptr<Blob> indexData) :
		m_format(format),
		m_indexData(indexData),
		m_indexBuffer(nullptr)
	{

	}

	IndexBuffer::~IndexBuffer()
	{
		SAFE_RELEASE(m_indexBuffer);
	}

	void IndexBuffer::doLoad()
	{
		if (!m_indexData)
		{
			loadFailed__();
			return;
		}

		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = (unsigned int)m_indexData->getLength();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = m_indexData->getData();

		HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBuffer(&ibd, &InitData, &m_indexBuffer);

		if (SUCCEEDED(hr))
		{
			loadSucceeded__();
		}
		else
		{
			loadFailed__();
		}
	}
}