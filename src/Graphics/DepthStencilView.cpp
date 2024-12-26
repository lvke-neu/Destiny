#include "DepthStencilView.h"
#include "Engine/Utility.h"
#include "Engine/Engine.h"
#include "GraphicsSystem.h"
#include <d3d11.h>

namespace Destiny
{
	DepthStencilView::DepthStencilView(unsigned int width, unsigned int height) :
		m_width(width),
		m_height(height),
		m_texture(nullptr),
		m_depthStencilView(nullptr),
		m_shaderResourceView(nullptr)
	{

	}

	DepthStencilView::~DepthStencilView()
	{
		SAFE_RELEASE(m_texture);
		SAFE_RELEASE(m_depthStencilView);
		SAFE_RELEASE(m_shaderResourceView);
	}

	void DepthStencilView::doLoad()
	{
		CD3D11_TEXTURE2D_DESC texDesc(DXGI_FORMAT_R24G8_TYPELESS,
			m_width, m_height, 1, 1,
			D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);

		HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateTexture2D(&texDesc, nullptr, &m_texture);
		if (!SUCCEEDED(hr))
		{
			
			LOG_ERROR("DepthStencilView:texture load failed:width{0},height{1}", m_width, m_height);
			loadFailed__();
			return;
		}

		CD3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc(m_texture, D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D24_UNORM_S8_UINT);
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateDepthStencilView(m_texture, &dsvDesc, &m_depthStencilView);
		if (!SUCCEEDED(hr))
		{
			
			LOG_ERROR("DepthStencilView:depthStencilView load failed:width{0},height{1}", m_width, m_height);
			loadFailed__();
			SAFE_RELEASE(m_texture);
			return;
		}

		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(m_texture, D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
		hr = Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateShaderResourceView(m_texture, &srvDesc, &m_shaderResourceView);
		if (!SUCCEEDED(hr))
		{
			
			LOG_ERROR("DepthStencilView:shaderResourceView load failed:width{0},height{1}", m_width, m_height);
			loadFailed__();
			SAFE_RELEASE(m_texture);
			SAFE_RELEASE(m_depthStencilView);
			return;
		}

		loadSucceeded__();
	}
}



