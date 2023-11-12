#include "IndexBuffer.h"
#include "Engine/Blob.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Engine/BlobHolder.h"
#include "GraphicsSystem.h"

namespace Destiny
{
	IndexBuffer::IndexBuffer() :
		m_format(DXGI_FORMAT_UNKNOWN),
		m_indexBuffer(nullptr)
	{

	}

	IndexBuffer::~IndexBuffer()
	{
		SAFE_RELEASE(m_indexBuffer);
	}

	void IndexBuffer::doLoad()
	{
		if (m_blobHolder)
		{
			if (m_blobHolder->isLoadingPending())
			{
				m_blobHolder->load(0);
			}

			if (!m_blobHolder->isLoadingSucceed())
			{
				loadFailed__();
				return;
			}

			auto blob = m_blobHolder->getBlob();
			if (blob)
			{
				memcpy_s(&m_format, sizeof(DXGI_FORMAT), blob->getData(), sizeof(DXGI_FORMAT));

				D3D11_BUFFER_DESC ibd;
				ZeroMemory(&ibd, sizeof(ibd));
				ibd.Usage = D3D11_USAGE_IMMUTABLE;
				ibd.ByteWidth = (unsigned int)(blob->getLength() - sizeof(DXGI_FORMAT));
				ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				ibd.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA InitData;
				ZeroMemory(&InitData, sizeof(InitData));
				InitData.pSysMem = (char*)blob->getData() + sizeof(DXGI_FORMAT);

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
			else
			{
				loadFailed__();
				return;
			}

		}
		else
		{
			loadFailed__();
		}


	}
}