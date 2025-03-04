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

	void ClearRenderTarget::execute(ID3D11DeviceContext* deviceContext)
	{
		if (!deviceContext)
		{
			return;
		}

		if (m_renderTargetView && m_depthStencilView)
		{
			static Color color{ 0, 0, 0, 255 };
			deviceContext->ClearRenderTargetView(*m_renderTargetView->getRenderTargetView(), (float*)&color);
			deviceContext->ClearDepthStencilView(m_depthStencilView->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}
}