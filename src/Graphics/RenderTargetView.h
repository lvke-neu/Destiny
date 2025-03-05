#pragma once
#include "Engine/Asset.h"

struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
namespace Destiny
{
	class Texture;
	class RenderTargetView : public Asset
	{
	public:
		RenderTargetView(unsigned int width, unsigned int height, unsigned int format = 28);
		virtual ~RenderTargetView();
	public:
		virtual void					doLoad() override;
		std::shared_ptr<Texture>		getTexture();
		ID3D11Texture2D*				getTexture2D();
		ID3D11RenderTargetView**		getRenderTargetView();
		ID3D11ShaderResourceView**		getShaderResourceView();
		unsigned int					getWidth();
		unsigned int					getHeight();
	private:
		ID3D11Texture2D*				m_texture;
		ID3D11RenderTargetView*			m_renderTargetView;
		ID3D11ShaderResourceView*		m_shaderResourceView;
		unsigned int					m_width;
		unsigned int					m_height;
		unsigned int					m_format;
	};

	inline ID3D11Texture2D* RenderTargetView::getTexture2D()
	{
		return m_texture;
	}

	inline ID3D11RenderTargetView** RenderTargetView::getRenderTargetView()
	{
		return &m_renderTargetView;
	}

	inline ID3D11ShaderResourceView** RenderTargetView::getShaderResourceView()
	{
		return &m_shaderResourceView;
	}

	inline unsigned int RenderTargetView::getWidth()
	{
		return m_width;
	}

	inline unsigned int RenderTargetView::getHeight()
	{
		return m_height;
	}
}