#include "BindRenderTargets.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include <d3d11.h>

namespace Destiny
{
	BindRenderTargets::BindRenderTargets() :
		m_renderTargetView(nullptr),
		m_depthStencilView(nullptr),
		m_viewPort(nullptr)
	{

	}

	BindRenderTargets::~BindRenderTargets()
	{
		
	}

	void BindRenderTargets::setRenderTargetView(unsigned int width, unsigned int height)
	{
		m_renderTargetView = std::make_shared<RenderTargetView>(width, height);
		m_renderTargetView->load(0);
	}

	void BindRenderTargets::setDepthStencilView(unsigned int width, unsigned int height)
	{
		m_depthStencilView = std::make_shared<DepthStencilView>(width, height);
		m_depthStencilView->load(0);
	}

	void BindRenderTargets::setViewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float naxDepth)
	{
		m_viewPort = std::make_shared<D3D11_VIEWPORT>();
		m_viewPort->TopLeftX = topLeftX;
		m_viewPort->TopLeftY = topLeftY;
		m_viewPort->Width = width;
		m_viewPort->Height = height;
		m_viewPort->MinDepth = minDepth;
		m_viewPort->MaxDepth = naxDepth;
	}

	void BindRenderTargets::execute(ID3D11DeviceContext* deviceContext)
	{
		if (!deviceContext)
		{
			return;
		}

		if(m_viewPort)
		{ 
			deviceContext->RSSetViewports(1, m_viewPort.get());
		}

		if (m_renderTargetView && m_depthStencilView)
		{
			deviceContext->OMSetRenderTargets(1, m_renderTargetView->getRenderTargetView(), m_depthStencilView->getDepthStencilView());
		}
	}
}