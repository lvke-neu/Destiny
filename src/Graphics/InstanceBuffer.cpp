#include "InstanceBuffer.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Graphics/GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny
{
	InstanceBuffer::InstanceBuffer(unsigned int byteWidth) :
		m_instanceBuffer(nullptr)
	{
		D3D11_BUFFER_DESC cbd;
		ZeroMemory(&cbd, sizeof(cbd));
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.ByteWidth = byteWidth;
		cbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBuffer(&cbd, nullptr, &m_instanceBuffer);
		if (FAILED(hr))
		{
			LOG_ERROR("Create ConstantBuffer Failed");
			return;
		}
	}

	InstanceBuffer::~InstanceBuffer()
	{
		SAFE_RELEASE(m_instanceBuffer);
	}
}