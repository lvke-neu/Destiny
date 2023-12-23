#pragma once
#include "Engine/Asset.h"
#include <d3d11.h>

namespace Destiny
{
	class Texture : public Asset
	{
		friend class TextureLoader;
	public:
		Texture();
		virtual ~Texture();
	public:
		ID3D11ShaderResourceView** getShaderResourceView();
		void create(ID3D11Resource* resource, ID3D11ShaderResourceView* shaderResourceView);
	private:
		ID3D11Resource* m_resource;
		ID3D11ShaderResourceView* m_shaderResourceView;
	};

	inline ID3D11ShaderResourceView** Texture::getShaderResourceView()
	{
		return &m_shaderResourceView;
	}

	inline void Texture::create(ID3D11Resource* resource, ID3D11ShaderResourceView* shaderResourceView)
	{
		if (!resource || !shaderResourceView)
		{
			loadFailed__();
			return;
		}

		resource->AddRef();
		shaderResourceView->AddRef();

		m_resource = resource;
		m_shaderResourceView = shaderResourceView;
		loadSucceeded__();
	}
}