#include "ConstantBuffer.h"
#include "GraphicsSystem.h"
#include "Engine/Utility.h"
#include "Engine/Blob.h"
#include <d3d11.h>

namespace Destiny
{
	ConstantBuffer::ConstantBuffer(unsigned int startSlot, unsigned int byteWidth) :
		m_startSlot(startSlot),
		m_byteWidth(byteWidth),
		m_constantBuffer(nullptr)
	{
		m_constantBufferBindFlag[ConstantBufferBindFlag::BindVS] = false;
		m_constantBufferBindFlag[ConstantBufferBindFlag::BindPS] = false;
		m_constantBufferBindFlag[ConstantBufferBindFlag::BindGS] = false;
		m_constantBufferBindFlag[ConstantBufferBindFlag::BindHS] = false;
		m_constantBufferBindFlag[ConstantBufferBindFlag::BindDS] = false;

		D3D11_BUFFER_DESC cbd;
		ZeroMemory(&cbd, sizeof(cbd));
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.ByteWidth = m_byteWidth;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		
		HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBuffer(&cbd, nullptr, &m_constantBuffer);
		if (FAILED(hr))
		{
			
			LOG_ERROR("Create ConstantBuffer Failed");
			return;
		}
		
		m_backData = std::make_shared<Blob>(m_byteWidth);
	}

	ConstantBuffer::~ConstantBuffer()
	{
		SAFE_RELEASE(m_constantBuffer);
	}

	void ConstantBuffer::setVariable(const std::string& name, std::shared_ptr<Blob> data)
	{
		auto iter = m_variables.find(name);
		if (iter == m_variables.end())
		{
			return;
		}

		//if (!m_constantBuffer ||!m_backData || !data || iter->second.size != data->getLength() || iter->second.offset + data->getLength() > m_byteWidth)
		//{
		//	return;
		//}

		if (!m_constantBuffer || !m_backData || !data || iter->second.offset + data->getLength() > m_byteWidth)
		{
			return;
		}

		D3D11_MAPPED_SUBRESOURCE mappedData;
		HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		if (SUCCEEDED(hr))
		{
			memcpy_s((char*)m_backData->getData() + iter->second.offset, data->getLength(), data->getData(), data->getLength());
			memcpy_s(mappedData.pData, m_backData->getLength(), m_backData->getData(), m_backData->getLength());
			Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->Unmap(m_constantBuffer, 0);
		}
	}

	void ConstantBuffer::setConstantBufferBindFlag(ConstantBufferBindFlag bindFlag, bool value)
	{
		m_constantBufferBindFlag[bindFlag] = value;
	}

	void ConstantBuffer::bind()
	{
		for (const auto& constantBufferBindFlag : m_constantBufferBindFlag)
		{
			if (constantBufferBindFlag.second)
			{
				switch (constantBufferBindFlag.first)
				{
				case ConstantBufferBindFlag::BindVS :
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetConstantBuffers(m_startSlot, 1, &m_constantBuffer);
					break;
				case ConstantBufferBindFlag::BindPS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetConstantBuffers(m_startSlot, 1, &m_constantBuffer);
					break;
				case ConstantBufferBindFlag::BindGS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->GSSetConstantBuffers(m_startSlot, 1, &m_constantBuffer);
					break;
				case ConstantBufferBindFlag::BindHS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->HSSetConstantBuffers(m_startSlot, 1, &m_constantBuffer);
					break;
				case ConstantBufferBindFlag::BindDS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->DSSetConstantBuffers(m_startSlot, 1, &m_constantBuffer);
					break;
				case ConstantBufferBindFlag::BindCS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->CSSetConstantBuffers(m_startSlot, 1, &m_constantBuffer);
					break;
				}
			}
		}
	}

	void ConstantBuffer::unBind()
	{
		ID3D11Buffer* buffer = nullptr;
		for (const auto& constantBufferBindFlag : m_constantBufferBindFlag)
		{
			if (constantBufferBindFlag.second)
			{
				switch (constantBufferBindFlag.first)
				{
				case ConstantBufferBindFlag::BindVS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetConstantBuffers(m_startSlot, 1, &buffer);
					break;
				case ConstantBufferBindFlag::BindPS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetConstantBuffers(m_startSlot, 1, &buffer);
					break;
				case ConstantBufferBindFlag::BindGS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->GSSetConstantBuffers(m_startSlot, 1, &buffer);
					break;
				case ConstantBufferBindFlag::BindHS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->HSSetConstantBuffers(m_startSlot, 1, &buffer);
					break;
				case ConstantBufferBindFlag::BindDS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->DSSetConstantBuffers(m_startSlot, 1, &buffer);
					break;
				case ConstantBufferBindFlag::BindCS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->CSSetConstantBuffers(m_startSlot, 1, &buffer);
					break;
				}
			}
		}
	}

	std::shared_ptr<ConstantBuffer> ConstantBuffer::shallowClone()
	{
		auto constantBuffer = std::make_shared<ConstantBuffer>(m_startSlot, m_byteWidth);
		constantBuffer->m_variables = m_variables;
		constantBuffer->m_constantBufferBindFlag = m_constantBufferBindFlag;

		return constantBuffer;
	}

	void ConstantBuffer::addVariable(const std::string& name, ConstantBufferVariable variable)
	{
		m_variables[name] = variable;
	}
}