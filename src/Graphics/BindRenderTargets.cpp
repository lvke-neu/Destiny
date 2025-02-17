#include "BindRenderTargets.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "Math/Color.h"
#include <d3d11.h>

namespace Destiny
{
	BindRenderTargets::BindRenderTargets() :
		m_renderTargetView(nullptr),
		m_depthStencilView(nullptr),
		m_viewPort(std::make_shared<D3D11_VIEWPORT>())
	{
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&BindRenderTargets::onResize, this, std::placeholders::_1));
	}

	BindRenderTargets::~BindRenderTargets()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&BindRenderTargets::onResize, this, std::placeholders::_1));
	}

	void BindRenderTargets::setRenderTargetView(std::shared_ptr<RenderTargetView> renderTargetView)
	{
		m_renderTargetView = renderTargetView;
	}

	std::shared_ptr<RenderTargetView> BindRenderTargets::getRenderTargetView()
	{
		return m_renderTargetView;
	}

	void BindRenderTargets::setDepthStencilView(std::shared_ptr<DepthStencilView> depthStencilView)
	{
		m_depthStencilView = depthStencilView;
	}

	std::shared_ptr<DepthStencilView> BindRenderTargets::getDepthStencilView()
	{
		return m_depthStencilView;
	}

	void BindRenderTargets::setViewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float naxDepth)
	{
		m_viewPort->TopLeftX = topLeftX;
		m_viewPort->TopLeftY = topLeftY;
		m_viewPort->Width	 = width;
		m_viewPort->Height   = height;
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
			static Color color{ 0, 0, 0, 255 };
			deviceContext->ClearRenderTargetView(*m_renderTargetView->getRenderTargetView(), (float*)&color);
			deviceContext->ClearDepthStencilView(m_depthStencilView->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}

	void BindRenderTargets::onResize(void* data)
	{
		WindowResizeData wrd = *(WindowResizeData*)data;
		if (!wrd.width || !wrd.height)
		{
			return;
		}

		setRenderTargetView(std::make_shared<Destiny::RenderTargetView>(wrd.width, wrd.height));
		getRenderTargetView()->load(0);
		setDepthStencilView(std::make_shared<Destiny::DepthStencilView>(wrd.width, wrd.height));
		getDepthStencilView()->load(0);
		setViewport(0.0f, 0.0f, (float)wrd.width, (float)wrd.height, 0.0f, 1.0f);
	}
}