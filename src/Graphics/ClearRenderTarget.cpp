#include "ClearRenderTarget.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Math/Color.h"
#include <d3d11.h>

namespace Destiny
{
	ClearRenderTarget::ClearRenderTarget() :
		m_renderTargetView(nullptr),
		m_depthStencilView(nullptr)
	{
		m_clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	}

	ClearRenderTarget::~ClearRenderTarget()
	{
		
	}

	void ClearRenderTarget::setRenderTargetView(std::shared_ptr<RenderTargetView> renderTargetView)
	{
		m_renderTargetView = renderTargetView;
	}

	void ClearRenderTarget::setDepthStencilView(std::shared_ptr<DepthStencilView> depthStencilView)
	{
		m_depthStencilView = depthStencilView;
	}

	void ClearRenderTarget::setClearColor(const Color& color)
	{
		m_clearColor = color;
	}

	void ClearRenderTarget::execute(ID3D11DeviceContext* deviceContext)
	{
		if (!deviceContext)
		{
			return;
		}

		if (m_renderTargetView && m_depthStencilView)
		{
			deviceContext->ClearRenderTargetView(*m_renderTargetView->getRenderTargetView(), (float*)&m_clearColor);
			deviceContext->ClearDepthStencilView(m_depthStencilView->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}
}