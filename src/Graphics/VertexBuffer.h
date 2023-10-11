#pragma once

struct ID3D11Device;
struct ID3D11Buffer;
namespace Destiny
{
	class VertexBuffer
	{
	public:
		VertexBuffer(ID3D11Device* device, unsigned int stride, unsigned int offset, const void* vertexData, unsigned int vertexDataSize);
		~VertexBuffer();
	public:
		ID3D11Buffer* m_pVertexBuffer;
		unsigned int m_stride;
		unsigned int m_offset;
	};
}