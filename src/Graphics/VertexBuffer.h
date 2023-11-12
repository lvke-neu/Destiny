#pragma once
#include "Engine/Asset.h"
#include <memory>
#include <vector>
#include <d3d11.h>

namespace Destiny
{
	class Blob;
	class VertexBuffer : public Asset
	{
	public:
		VertexBuffer(const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, unsigned int stride, unsigned int offset, std::shared_ptr<Blob> vertexData);
		virtual ~VertexBuffer();
	public:
		virtual void doLoad() override;
	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_inputElements;
		unsigned int m_stride;
		unsigned int m_offset;
		std::shared_ptr<Blob> m_vertexData;
		ID3D11Buffer* m_vertexBuffer;
	};
}