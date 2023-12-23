#pragma once
#include "Engine/Asset.h"

struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
namespace Destiny
{
	class RenderTargetView : public Asset
	{
	public:
		RenderTargetView(unsigned int width, unsigned int height);
		virtual ~RenderTargetView();
	public:
		virtual void doLoad() override;
		ID3D11Texture2D* getTexture2D();
		ID3D11RenderTargetView* getRenderTargetView();
		ID3D11ShaderResourceView** getShaderResourceView();
	private:
		ID3D11Texture2D* m_texture;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11ShaderResourceView* m_shaderResourceView;
		unsigned int m_width;
		unsigned int m_height;
	};

	inline ID3D11Texture2D* RenderTargetView::getTexture2D()
	{
		return m_texture;
	}

	inline ID3D11RenderTargetView* RenderTargetView::getRenderTargetView()
	{
		return m_renderTargetView;
	}

	inline ID3D11ShaderResourceView** RenderTargetView::getShaderResourceView()
	{
		return &m_shaderResourceView;
	}
}