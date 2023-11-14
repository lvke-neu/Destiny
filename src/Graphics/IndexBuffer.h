#pragma once
#include "Engine/Asset.h"
#include <d3d11.h>

namespace Destiny
{
	class Blob;
	class IndexBuffer : public Asset
	{
	public:
		IndexBuffer();
		virtual ~IndexBuffer();
	public:
		virtual void doLoad() override;
		ID3D11Buffer* getIndexBuffer();
		DXGI_FORMAT getFormat();
		unsigned int getCount();
	private:
		DXGI_FORMAT m_format;
		ID3D11Buffer* m_indexBuffer;
		unsigned int m_count;
	};

	inline ID3D11Buffer* IndexBuffer::getIndexBuffer()
	{
		return m_indexBuffer;
	}

	inline DXGI_FORMAT IndexBuffer::getFormat()
	{
		return m_format;
	}

	inline unsigned int IndexBuffer::getCount()
	{
		return m_count;
	}
}