#pragma once
#include <d3d11.h>
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"

namespace Destiny
{
	template<typename T>
	class ConstantBuffer
	{
	public:
		ConstantBuffer();
		~ConstantBuffer();
	public:
		void update(T data);
		ID3D11Buffer** getConstantBuffer()
		{
			return &m_constantBuffer;
		}
	private:
		ID3D11Buffer* m_constantBuffer;
	};

	template<typename T>
	ConstantBuffer<T>::ConstantBuffer() :
		m_constantBuffer(nullptr)
	{
		D3D11_BUFFER_DESC cbd;
		ZeroMemory(&cbd, sizeof(cbd));
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.ByteWidth = sizeof(T);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBuffer(&cbd, nullptr, &m_constantBuffer);
	}

	template<typename T>
	ConstantBuffer<T>::~ConstantBuffer()
	{
		SAFE_RELEASE(m_constantBuffer);
	}

	template<typename T>
	void ConstantBuffer<T>::update(T data)
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		memcpy_s(mappedData.pData, sizeof(T), &data, sizeof(T));
		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->Unmap(m_constantBuffer, 0);
	}
}