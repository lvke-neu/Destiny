#include "RenderTargetView.h"
#include "GraphicsSystem.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "Texture.h"
#include <d3d11.h>

namespace Destiny 
{
	RenderTargetView::RenderTargetView(unsigned int width, unsigned int height, unsigned int format) :
		m_width(width), 
		m_height(height),
		m_texture(nullptr),
		m_renderTargetView(nullptr),
		m_shaderResourceView(nullptr),
		m_format(format)
	{
		
	}

	RenderTargetView::~RenderTargetView()
	{
		SAFE_RELEASE(m_texture);
		SAFE_RELEASE(m_renderTargetView);
		SAFE_RELEASE(m_shaderResourceView);
	}

	void RenderTargetView::doLoad()
	{
		CD3D11_TEXTURE2D_DESC texDesc((DXGI_FORMAT)m_format, m_width, m_height, 1,
			1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			D3D11_USAGE_DEFAULT, 0, 1, 0, 0);

		HRESULT hr;

		

		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateTexture2D(&texDesc, nullptr, &m_texture);
		if (!SUCCEEDED(hr))
		{
			
			LOG_ERROR("RenderTargetView:texture load failed:width{0},height{1}", m_width, m_height);
			loadFailed__();
			return;
		}


		CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(m_texture, D3D11_RTV_DIMENSION_TEXTURE2D);
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateRenderTargetView(m_texture, &renderTargetViewDesc, &m_renderTargetView);
		if (!SUCCEEDED(hr))
		{
			
			LOG_ERROR("RenderTargetView:renderTargetView load failed:width{0},height{1}", m_width, m_height);
			loadFailed__();
			SAFE_RELEASE(m_texture);
			return;
		}

		CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc(m_texture, D3D11_SRV_DIMENSION_TEXTURE2D);
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateShaderResourceView(m_texture, &shaderResourceViewDesc, &m_shaderResourceView);
		if (!SUCCEEDED(hr))
		{
			
			LOG_ERROR("RenderTargetView:shaderResourceView load failed:width{0},height{1}", m_width, m_height);
			loadFailed__();
			SAFE_RELEASE(m_texture);
			SAFE_RELEASE(m_renderTargetView);
			return;
		}
		
		loadSucceeded__();
	}

	std::shared_ptr<Texture> RenderTargetView::getTexture()
	{
		auto texture = std::make_shared<Texture>(m_texture, m_shaderResourceView);
		return texture;
	}
}



