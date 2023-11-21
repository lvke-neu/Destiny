#pragma once
#include "Engine/Asset.h"
#include <d3d11.h>

namespace Destiny
{
	class Texture : public Asset
	{
		friend class TextureLoader_color;
		friend class TextureLoader_dds;
	public:
		Texture();
		virtual ~Texture();
	private:
		ID3D11Resource* m_resource;
		ID3D11ShaderResourceView* m_shaderResourceView;
	};
}