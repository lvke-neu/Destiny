#include "DeferredOpaquePipeline.h"
#include "BindRenderTargets.h"
#include "ClearRenderTarget.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "DrawCommand.h"
#include "DrawParameters.h"
#include "SamplerState.h"
#include "Texture.h"
#include "Renderer.h"
#include "RenderStates.h"
#include "RenderPass.h"
#include "InputLayout.h"
#include "Mesh.h"
#include "MeshProvider.h"
#include "RenderSystem.h"
#include "Visual.h"
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
		m_clearRenderTarget2(std::make_shared<ClearRenderTarget>()),
		m_clearRenderTarget3(std::make_shared<ClearRenderTarget>()),
		m_clearRenderTarget4(std::make_shared<ClearRenderTarget>()),
		m_clearRenderTarget5(std::make_shared<ClearRenderTarget>()),
		m_clearRenderTarget6(std::make_shared<ClearRenderTarget>()),
		m_fullScreenTriangle(std::make_shared<Visual>())
	{
		m_clearRenderTarget5->setClearColor({ 0.0f, 0.0f, 1.0f, 1.0f });


		auto samplerState = std::make_shared<SamplerState>();
		//samplerState->getSamplerDesc()->Filter = D3D11_FILTER_ANISOTROPIC;
		samplerState->load(0);

		auto renderer = Renderer::Create("builtin://renderer/deferred_full_screen_triangle.hlsl");
		renderer->load(0);
	
		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->getDepthStencilStateDesc()->DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		renderStates->load(0);

		auto mesh = MeshProvider::Create_FullScreenTriangle();
		mesh->load(0);

		auto renderPass = std::make_shared<RenderPass>();
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		m_fullScreenTriangle->setMesh(mesh);
		m_fullScreenTriangle->setRenderPass(renderPass);
		m_fullScreenTriangle->setSamplerSate("s_sampler", samplerState);

		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&DeferredOpaquePipeline::onResize, this, std::placeholders::_1));
	}

	DeferredOpaquePipeline::~DeferredOpaquePipeline()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&DeferredOpaquePipeline::onResize, this, std::placeholders::_1));
	}

	void DeferredOpaquePipeline::execute(std::shared_ptr<GraphicsContext> deviceContext)
	{
#if 0
		//GBuffer
		m_bindRenderTargets->execute(deviceContext);
		m_clearRenderTarget0->execute(deviceContext);
		m_clearRenderTarget1->execute(deviceContext);
		m_clearRenderTarget2->execute(deviceContext);
		m_clearRenderTarget3->execute(deviceContext);
		m_clearRenderTarget4->execute(deviceContext);
		m_clearRenderTarget5->execute(deviceContext);
		m_clearRenderTarget6->execute(deviceContext);

		//Draw DeferredOpaquePipeline Object 
		GraphicsCommandList::execute(deviceContext);

		//full screen triangle
		if (m_renderSystem && m_renderSystem->m_bindRenderTargets)
		{
			m_renderSystem->m_bindRenderTargets->execute(deviceContext);
		}
		if (m_bindRenderTargets->getRenderTargetViews(0))
		{
			m_fullScreenTriangle->setShaderResource("t_albedo", m_bindRenderTargets->getRenderTargetViews(0)->getTexture());
		}
		if (m_bindRenderTargets->getRenderTargetViews(1))
		{
			m_fullScreenTriangle->setShaderResource("t_normal", m_bindRenderTargets->getRenderTargetViews(1)->getTexture());
		}
		if (m_bindRenderTargets->getRenderTargetViews(2))
		{
			m_fullScreenTriangle->setShaderResource("t_mra", m_bindRenderTargets->getRenderTargetViews(2)->getTexture());
		}
		if (m_bindRenderTargets->getRenderTargetViews(3))
		{
			m_fullScreenTriangle->setShaderResource("t_positionW", m_bindRenderTargets->getRenderTargetViews(3)->getTexture());
		}
		if (m_bindRenderTargets->getRenderTargetViews(4))
		{
			m_fullScreenTriangle->setShaderResource("t_normalW", m_bindRenderTargets->getRenderTargetViews(4)->getTexture());
		}
		if (m_bindRenderTargets->getRenderTargetViews(5))
		{
			m_fullScreenTriangle->setShaderResource("t_texcoord", m_bindRenderTargets->getRenderTargetViews(5)->getTexture());
		}
		if (m_bindRenderTargets->getRenderTargetViews(6))
		{
			m_fullScreenTriangle->setShaderResource("t_shadowPosH", m_bindRenderTargets->getRenderTargetViews(6)->getTexture());
		}
		
		m_fullScreenTriangle->updateDrawParameters();
		m_fullScreenTriangle->execute(deviceContext);
#endif
	}

	void DeferredOpaquePipeline::onRendererConstantChanged()
	{
		m_fullScreenTriangle->setRendererConstantChanged();
	}

	void DeferredOpaquePipeline::onRendererTexturesChanged()
	{
		m_fullScreenTriangle->setRendererTexturesChanged();
	}

	void DeferredOpaquePipeline::onRendererSamplerStatesChanged()
	{
		m_fullScreenTriangle->setRendererSamplerStatesChanged();
	}

	void DeferredOpaquePipeline::onResize(void* data)
	{
		WindowResizeData wrd = *(WindowResizeData*)data;
		if (!wrd.width || !wrd.height)
		{
			return;
		}
		//DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
		auto renderTargetView0 = std::make_shared<RenderTargetView>(wrd.width, wrd.height, 2);
		renderTargetView0->load(0);
		auto renderTargetView1 = std::make_shared<RenderTargetView>(wrd.width, wrd.height, 2);
		renderTargetView1->load(0);
		auto renderTargetView2 = std::make_shared<RenderTargetView>(wrd.width, wrd.height, 2);
		renderTargetView2->load(0);
		auto renderTargetView3 = std::make_shared<RenderTargetView>(wrd.width, wrd.height, 2);
		renderTargetView3->load(0);
		auto renderTargetView4 = std::make_shared<RenderTargetView>(wrd.width, wrd.height, 2);
		renderTargetView4->load(0);
		auto renderTargetView5 = std::make_shared<RenderTargetView>(wrd.width, wrd.height, 2);
		renderTargetView5->load(0);
		auto renderTargetView6 = std::make_shared<RenderTargetView>(wrd.width, wrd.height, 2);
		renderTargetView6->load(0);

		auto depthStencilView0 = std::make_shared<DepthStencilView>(wrd.width, wrd.height);
		depthStencilView0->load(0);
		auto depthStencilView1 = std::make_shared<DepthStencilView>(wrd.width, wrd.height);
		depthStencilView1->load(0);
		auto depthStencilView2 = std::make_shared<DepthStencilView>(wrd.width, wrd.height);
		depthStencilView2->load(0);
		auto depthStencilView3 = std::make_shared<DepthStencilView>(wrd.width, wrd.height);
		depthStencilView3->load(0);
		auto depthStencilView4 = std::make_shared<DepthStencilView>(wrd.width, wrd.height);
		depthStencilView4->load(0);
		auto depthStencilView5 = std::make_shared<DepthStencilView>(wrd.width, wrd.height);
		depthStencilView5->load(0);
		auto depthStencilView6 = std::make_shared<DepthStencilView>(wrd.width, wrd.height);
		depthStencilView6->load(0);

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
		auto viewPort3 = std::make_shared<D3D11_VIEWPORT>();
		viewPort3->TopLeftX = 0.0f;
		viewPort3->TopLeftY = 0.0f;
		viewPort3->Width = (float)wrd.width;
		viewPort3->Height = (float)wrd.height;
		viewPort3->MinDepth = 0.0f;
		viewPort3->MaxDepth = 1.0f;
		auto viewPort4 = std::make_shared<D3D11_VIEWPORT>();
		viewPort4->TopLeftX = 0.0f;
		viewPort4->TopLeftY = 0.0f;
		viewPort4->Width = (float)wrd.width;
		viewPort4->Height = (float)wrd.height;
		viewPort4->MinDepth = 0.0f;
		viewPort4->MaxDepth = 1.0f;
		auto viewPort5 = std::make_shared<D3D11_VIEWPORT>();
		viewPort5->TopLeftX = 0.0f;
		viewPort5->TopLeftY = 0.0f;
		viewPort5->Width = (float)wrd.width;
		viewPort5->Height = (float)wrd.height;
		viewPort5->MinDepth = 0.0f;
		viewPort5->MaxDepth = 1.0f;
		auto viewPort6 = std::make_shared<D3D11_VIEWPORT>();
		viewPort6->TopLeftX = 0.0f;
		viewPort6->TopLeftY = 0.0f;
		viewPort6->Width = (float)wrd.width;
		viewPort6->Height = (float)wrd.height;
		viewPort6->MinDepth = 0.0f;
		viewPort6->MaxDepth = 1.0f;

		std::vector<std::shared_ptr<RenderTargetView>>	renderTargetViews;
		renderTargetViews.push_back(renderTargetView0);
		renderTargetViews.push_back(renderTargetView1);
		renderTargetViews.push_back(renderTargetView2);
		renderTargetViews.push_back(renderTargetView3);
		renderTargetViews.push_back(renderTargetView4);
		renderTargetViews.push_back(renderTargetView5);
		renderTargetViews.push_back(renderTargetView6);
		std::vector<std::shared_ptr<DepthStencilView>>	depthStencilViews;
		depthStencilViews.push_back(depthStencilView0);
		depthStencilViews.push_back(depthStencilView1);
		depthStencilViews.push_back(depthStencilView2);
		depthStencilViews.push_back(depthStencilView3);
		depthStencilViews.push_back(depthStencilView4);
		depthStencilViews.push_back(depthStencilView5);
		depthStencilViews.push_back(depthStencilView6);
		std::vector<std::shared_ptr<D3D11_VIEWPORT>>	viewPorts;
		viewPorts.push_back(viewPort0);
		viewPorts.push_back(viewPort1);
		viewPorts.push_back(viewPort2);
		viewPorts.push_back(viewPort3);
		viewPorts.push_back(viewPort4);
		viewPorts.push_back(viewPort5);
		viewPorts.push_back(viewPort6);

		m_bindRenderTargets->setRenderTargetViews(renderTargetViews);
		m_bindRenderTargets->setDepthStencilViews(depthStencilViews);
		m_bindRenderTargets->setViewports(viewPorts);

		m_clearRenderTarget0->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(0));
		m_clearRenderTarget0->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(0));
		m_clearRenderTarget1->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(1));
		m_clearRenderTarget1->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(1));
		m_clearRenderTarget2->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(2));
		m_clearRenderTarget2->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(2));
		m_clearRenderTarget3->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(3));
		m_clearRenderTarget3->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(3));
		m_clearRenderTarget4->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(4));
		m_clearRenderTarget4->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(4));
		m_clearRenderTarget5->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(5));
		m_clearRenderTarget5->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(5));		
		m_clearRenderTarget6->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(6));
		m_clearRenderTarget6->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(6));
	}
}