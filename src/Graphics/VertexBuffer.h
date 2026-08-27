#pragma once
#include "Engine/Asset.h"

struct ID3D11Buffer;
namespace Destiny
{
	class Blob;
	class InputLayout;
	class VertexBuffer : public Asset
	{
		friend class Mesh;
	public:
		VertexBuffer(std::shared_ptr<InputLayout> inputLayout, unsigned int stride, unsigned int offset, std::shared_ptr<Blob> data);
		virtual ~VertexBuffer();
	public:
		virtual void doLoad() override;
		void modify(std::shared_ptr<Blob> data);
	private:
		std::shared_ptr<InputLayout>	m_inputLayout;
		unsigned int					m_stride;
		unsigned int					m_offset;
		ID3D11Buffer*					m_vertexBuffer;
		std::shared_ptr<Blob>			m_data;
	};
}