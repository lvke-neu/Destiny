#pragma once
#include "Engine/Asset.h"
#include <d3d11.h>

namespace Destiny
{
	class Blob;
	class VertexBuffer : public Asset
	{
	public:
		VertexBuffer();
		virtual ~VertexBuffer();
	public:
		virtual void doLoad() override;
		ID3D11Buffer** getVertexBuffer();
		unsigned int* getStride();
		unsigned int* getOffset();
	private:
		unsigned int m_stride;
		unsigned int m_offset;
		ID3D11Buffer* m_vertexBuffer;
	};

	inline ID3D11Buffer** VertexBuffer::getVertexBuffer()
	{
		return &m_vertexBuffer;
	}

	inline unsigned int* VertexBuffer::getStride()
	{
		return &m_stride;
	}

	inline unsigned int* VertexBuffer::getOffset()
	{
		return &m_offset;
	}
}