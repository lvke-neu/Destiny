#pragma once
#include "Engine/Asset.h"

struct ID3D11Buffer;
namespace Destiny
{
	class Blob;
	class IndexBuffer : public Asset
	{
		friend class Mesh;
	public:
		enum class IndexType
		{
			Index16 = 57,
			Index32 = 42
		};
	public:
		IndexBuffer(IndexType indexType, std::shared_ptr<Blob> data);
		virtual ~IndexBuffer();
	public:
		virtual void doLoad() override;
	private:
		IndexType				m_indexType;
		ID3D11Buffer*			m_indexBuffer;
		std::shared_ptr<Blob>	m_data;
	};

}