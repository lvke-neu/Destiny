#include "RenderTargetView.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny 
{
	RenderTargetView::RenderTargetView(unsigned int width, unsigned int height) :
		m_width(width), 
		m_height(height),
		m_texture(nullptr),
		m_renderTargetView(nullptr),
		m_shaderResourceView(nullptr)
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
		CD3D11_TEXTURE2D_DESC texDesc(DXGI_FORMAT_R8G8B8A8_UNORM, m_width, m_height, 1,
			1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			D3D11_USAGE_DEFAULT, 0, 1, 0, 0);

		HRESULT hr;

		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateTexture2D(&texDesc, nullptr, &m_texture);
		if (!SUCCEEDED(hr))
		{
			LOG_ERROR("RenderTargetView:texture load failed");
			loadFailed__();
			return;
		}


		CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(m_texture, D3D11_RTV_DIMENSION_TEXTURE2D);
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateRenderTargetView(m_texture, &renderTargetViewDesc, &m_renderTargetView);
		if (!SUCCEEDED(hr))
		{
			LOG_ERROR("RenderTargetView:renderTargetView load failed");
			loadFailed__();
			SAFE_RELEASE(m_texture);
			return;
		}

		CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc(m_texture, D3D11_SRV_DIMENSION_TEXTURE2D);
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateShaderResourceView(m_texture, &shaderResourceViewDesc, &m_shaderResourceView);
		if (!SUCCEEDED(hr))
		{
			LOG_ERROR("RenderTargetView:shaderResourceView load failed");
			loadFailed__();
			SAFE_RELEASE(m_texture);
			SAFE_RELEASE(m_renderTargetView);
			return;
		}

		loadSucceeded__();
	}
}



