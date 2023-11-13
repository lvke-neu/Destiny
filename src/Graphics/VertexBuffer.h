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
	private:
		unsigned int m_stride;
		unsigned int m_offset;
		ID3D11Buffer* m_vertexBuffer;
	};
}