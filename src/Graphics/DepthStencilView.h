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
		void*							getTexture2D();
		void*							getDepthStencilView();
		void**							getShaderResourceView();
	private:
		void*							m_texture;
		void*							m_depthStencilView;
		void*							m_shaderResourceView;
		unsigned int					m_width;
		unsigned int					m_height;
	};

	inline void* DepthStencilView::getTexture2D()
	{
		return m_texture;
	}

	inline void* DepthStencilView::getDepthStencilView()
	{
		return m_depthStencilView;
	}

	inline void** DepthStencilView::getShaderResourceView()
	{
		return &m_shaderResourceView;
	}
}