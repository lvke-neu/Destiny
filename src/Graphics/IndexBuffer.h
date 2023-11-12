#pragma once
#include "Engine/Asset.h"
#include <memory>
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
	private:
		DXGI_FORMAT m_format;
		ID3D11Buffer* m_indexBuffer;
	};
}