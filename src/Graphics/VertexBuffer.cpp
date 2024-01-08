#include "VertexBuffer.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"

namespace Destiny
{
	VertexBuffer::VertexBuffer() :
		m_stride(0),
		m_offset(0),
		m_vertexBuffer(nullptr),
		m_count(0)
	{

	}

	VertexBuffer::~VertexBuffer()
	{
		SAFE_RELEASE(m_vertexBuffer);
	}

	void VertexBuffer::doLoad()
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
				memcpy_s(&m_stride, sizeof(unsigned int), blob->getData(), sizeof(unsigned int));
				memcpy_s(&m_offset, sizeof(unsigned int), (char*)blob->getData() + sizeof(unsigned int), sizeof(unsigned int));

				D3D11_BUFFER_DESC vbd;
				ZeroMemory(&vbd, sizeof(vbd));
				vbd.Usage = D3D11_USAGE_IMMUTABLE;
				vbd.ByteWidth = (unsigned int)(blob->getLength() - sizeof(unsigned int) * 2);
				vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				vbd.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA InitData;
				ZeroMemory(&InitData, sizeof(InitData));
				InitData.pSysMem = (char*)blob->getData() + sizeof(unsigned int) * 2;

				m_count = (unsigned int)(blob->getLength() - sizeof(unsigned int) * 2) / m_stride;

				HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBuffer(&vbd, &InitData, &m_vertexBuffer);

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