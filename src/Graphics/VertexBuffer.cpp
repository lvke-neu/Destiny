#include "VertexBuffer.h"
#include "Engine/Blob.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"

namespace Destiny
{
	VertexBuffer::VertexBuffer(const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, unsigned int stride, unsigned int offset, std::shared_ptr<Blob> vertexData) :
		m_inputElements(inputElements),
		m_stride(stride),
		m_offset(offset),
		m_vertexData(vertexData),
		m_vertexBuffer(nullptr)
	{

	}

	VertexBuffer::~VertexBuffer()
	{
		SAFE_RELEASE(m_vertexBuffer);
	}

	void VertexBuffer::doLoad()
	{
		if (!m_vertexData)
		{
			loadFailed__();
			return;
		}

		D3D11_BUFFER_DESC vbd;
		ZeroMemory(&vbd, sizeof(vbd));
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = (unsigned int)m_vertexData->getLength();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = m_vertexData->getData();

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
}