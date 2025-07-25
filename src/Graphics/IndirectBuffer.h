#pragma once
#include "Engine/Asset.h"

struct ID3D11Buffer;
namespace Destiny
{
	class Blob;
	class IndirectBuffer : public Asset
	{
		friend class Mesh;
	public:
		IndirectBuffer(std::shared_ptr<Blob> data);
		virtual ~IndirectBuffer();
	public:
		virtual void doLoad() override;
	private:
		ID3D11Buffer*					m_indirectBuffer;
		std::shared_ptr<Blob>			m_data;
	};
}