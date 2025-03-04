#include "DeferredOpaquePipeline.h"
#include "BindRenderTargets.h"
#include "ClearRenderTarget.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Engine/EventSystem.h"
#include "Engine/Engine.h"
#include <d3d11.h>

namespace Destiny
{
	DeferredOpaquePipeline::DeferredOpaquePipeline(std::shared_ptr<RenderSystem> renderSystem) :
		GraphicsPipeline(renderSystem),
		m_bindRenderTargets(std::make_shared<BindRenderTargets>()),
		m_clearRenderTarget0(std::make_shared<ClearRenderTarget>()),
		m_clearRenderTarget1(std::make_shared<ClearRenderTarget>()),
		m_clearRenderTarget2(std::make_shared<ClearRenderTarget>())
	{
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&DeferredOpaquePipeline::onResize, this, std::placeholders::_1));
	}

	DeferredOpaquePipeline::~DeferredOpaquePipeline()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&DeferredOpaquePipeline::onResize, this, std::placeholders::_1));
	}

	void DeferredOpaquePipeline::execute(ID3D11DeviceContext* deviceContext)
	{
		m_bindRenderTargets->execute(deviceContext);
		m_clearRenderTarget0->execute(deviceContext);
		m_clearRenderTarget1->execute(deviceContext);
		m_clearRenderTarget2->execute(deviceContext);

		GraphicsCommandList::execute(deviceContext);
	}

	void DeferredOpaquePipeline::onResize(void* data)
	{
		WindowResizeData wrd = *(WindowResizeData*)data;
		if (!wrd.width || !wrd.height)
		{
			return;
		}

		auto renderTargetView0 = std::make_shared<RenderTargetView>(wrd.width, wrd.height);
		renderTargetView0->load(0);
		auto renderTargetView1 = std::make_shared<RenderTargetView>(wrd.width, wrd.height);
		renderTargetView1->load(0);
		auto renderTargetView2 = std::make_shared<RenderTargetView>(wrd.width, wrd.height);
		renderTargetView2->load(0);

		auto depthStencilView0 = std::make_shared<DepthStencilView>(wrd.width, wrd.height);
		depthStencilView0->load(0);
		auto depthStencilView1 = std::make_shared<DepthStencilView>(wrd.width, wrd.height);
		depthStencilView1->load(0);
		auto depthStencilView2 = std::make_shared<DepthStencilView>(wrd.width, wrd.height);
		depthStencilView2->load(0);

		auto viewPort0 = std::make_shared<D3D11_VIEWPORT>();
		viewPort0->TopLeftX = 0.0f;
		viewPort0->TopLeftY = 0.0f;
		viewPort0->Width = (float)wrd.width;
		viewPort0->Height = (float)wrd.height;
		viewPort0->MinDepth = 0.0f;
		viewPort0->MaxDepth = 1.0f;
		auto viewPort1 = std::make_shared<D3D11_VIEWPORT>();
		viewPort1->TopLeftX = 0.0f;
		viewPort1->TopLeftY = 0.0f;
		viewPort1->Width = (float)wrd.width;
		viewPort1->Height = (float)wrd.height;
		viewPort1->MinDepth = 0.0f;
		viewPort1->MaxDepth = 1.0f;
		auto viewPort2 = std::make_shared<D3D11_VIEWPORT>();
		viewPort2->TopLeftX = 0.0f;
		viewPort2->TopLeftY = 0.0f;
		viewPort2->Width = (float)wrd.width;
		viewPort2->Height = (float)wrd.height;
		viewPort2->MinDepth = 0.0f;
		viewPort2->MaxDepth = 1.0f;

		std::vector<std::shared_ptr<RenderTargetView>>	renderTargetViews;
		renderTargetViews.push_back(renderTargetView0);
		renderTargetViews.push_back(renderTargetView1);
		renderTargetViews.push_back(renderTargetView2);
		std::vector<std::shared_ptr<DepthStencilView>>	depthStencilViews;
		depthStencilViews.push_back(depthStencilView0);
		depthStencilViews.push_back(depthStencilView1);
		depthStencilViews.push_back(depthStencilView2);
		std::vector<std::shared_ptr<D3D11_VIEWPORT>>	viewPorts;
		viewPorts.push_back(viewPort0);
		viewPorts.push_back(viewPort1);
		viewPorts.push_back(viewPort2);

		m_bindRenderTargets->setRenderTargetViews(renderTargetViews);
		m_bindRenderTargets->setDepthStencilViews(depthStencilViews);
		m_bindRenderTargets->setViewports(viewPorts);

		m_clearRenderTarget0->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(0));
		m_clearRenderTarget0->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(0));
		m_clearRenderTarget1->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(1));
		m_clearRenderTarget1->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(1));
		m_clearRenderTarget2->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(2));
		m_clearRenderTarget2->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(2));
	}
}