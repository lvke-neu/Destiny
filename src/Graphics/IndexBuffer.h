#pragma once
#include <dxgiformat.h>

struct ID3D11Device;
struct ID3D11Buffer;
namespace Destiny
{
	class IndexBuffer
	{
	public:
		IndexBuffer(ID3D11Device* device, DXGI_FORMAT format, const void* indexData, unsigned int indexDataSize);
		~IndexBuffer();
	public:
		ID3D11Buffer* m_pIndexBuffer;
		DXGI_FORMAT m_format;
	};
}