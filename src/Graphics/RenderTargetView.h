#pragma once
#include "Engine/Asset.h"
#include <string>

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
		void*							getTexture2D();
		void**							getRenderTargetView();
		void**							getShaderResourceView();
		unsigned int					getWidth();
		unsigned int					getHeight();
		void setDebugObjectName(const std::string& name);
	private:
		void*							m_texture;
		void*							m_renderTargetView;
		void*							m_shaderResourceView;
		unsigned int					m_width;
		unsigned int					m_height;
		unsigned int					m_format;
	};

	inline void* RenderTargetView::getTexture2D()
	{
		return m_texture;
	}

	inline void** RenderTargetView::getRenderTargetView()
	{
		return &m_renderTargetView;
	}

	inline void** RenderTargetView::getShaderResourceView()
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