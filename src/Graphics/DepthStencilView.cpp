#include "DepthStencilView.h"
#include "Texture.h"
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
		// SAFE_RELEASE(m_texture);
		// SAFE_RELEASE(m_depthStencilView);
		// SAFE_RELEASE(m_shaderResourceView);

        if (m_texture)
        {
             Engine::GetInstance()->getGraphicsSystem()->getDevice()->DestroyTexture(m_texture);
             m_texture = nullptr;
        }
        
        m_depthStencilView = nullptr;
        m_shaderResourceView = nullptr;
	}

	void DepthStencilView::doLoad()
	{
		CD3D11_TEXTURE2D_DESC texDesc(DXGI_FORMAT_R24G8_TYPELESS,
			m_width, m_height, 1, 1,
			D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);

		HRESULT hr = (HRESULT)Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateTexture2D(&texDesc, nullptr, (void**)&m_texture);
		if (!SUCCEEDED(hr))
		{
			
			LOG_ERROR("DepthStencilView:texture load failed:width{0},height{1}", m_width, m_height);
			loadFailed__();
			return;
		}

		CD3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D24_UNORM_S8_UINT);
		hr = (HRESULT)Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateDepthStencilView(m_texture, &dsvDesc, (void**)&m_depthStencilView);
		if (!SUCCEEDED(hr))
		{
			
			LOG_ERROR("DepthStencilView:depthStencilView load failed:width{0},height{1}", m_width, m_height);
			loadFailed__();
			// SAFE_RELEASE(m_texture);
             Engine::GetInstance()->getGraphicsSystem()->getDevice()->DestroyTexture(m_texture);
             m_texture = nullptr;
			return;
		}

		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
		hr = (HRESULT)Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateShaderResourceView(m_texture, &srvDesc, (void**)&m_shaderResourceView);
		if (!SUCCEEDED(hr))
		{
			
			LOG_ERROR("DepthStencilView:shaderResourceView load failed:width{0},height{1}", m_width, m_height);
			loadFailed__();
			// SAFE_RELEASE(m_texture);
			// SAFE_RELEASE(m_depthStencilView);
            Engine::GetInstance()->getGraphicsSystem()->getDevice()->DestroyTexture(m_texture);
            m_texture = nullptr;
            m_depthStencilView = nullptr;
			return;
		}

		loadSucceeded__();
	}

	std::shared_ptr<Texture> DepthStencilView::getTexture()
	{
		auto texture = std::make_shared<Texture>(m_texture, m_shaderResourceView);
		return texture;
	}
}



