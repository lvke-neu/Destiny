#pragma once
#include "Engine/Asset.h"

struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
namespace Destiny
{
	class Texture;
	class DepthStencilView : public Asset
	{
	public:
		DepthStencilView(unsigned int width, unsigned int height);
		virtual ~DepthStencilView();
	public:
		virtual void					doLoad() override;
		std::shared_ptr<Texture>		getTexture();
		ID3D11Texture2D*				getTexture2D();
		ID3D11DepthStencilView*			getDepthStencilView();
		ID3D11ShaderResourceView**		getShaderResourceView();
	private:
		ID3D11Texture2D*				m_texture;
		ID3D11DepthStencilView*			m_depthStencilView;
		ID3D11ShaderResourceView*		m_shaderResourceView;
		unsigned int					m_width;
		unsigned int					m_height;
	};

	inline ID3D11Texture2D* DepthStencilView::getTexture2D()
	{
		return m_texture;
	}

	inline ID3D11DepthStencilView* DepthStencilView::getDepthStencilView()
	{
		return m_depthStencilView;
	}

	inline ID3D11ShaderResourceView** DepthStencilView::getShaderResourceView()
	{
		return &m_shaderResourceView;
	}
}