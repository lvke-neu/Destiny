#include "PostProcessingPipeline.h"
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
#include "ComputerCommand.h"
#include "Engine/EventSystem.h"
#include "Engine/Engine.h"
#include <d3d11.h>

namespace Destiny
{
	PostProcessingPipeline::PostProcessingPipeline(std::shared_ptr<RenderSystem> renderSystem) :
		GraphicsPipeline(renderSystem),
		m_bindRenderTargets(std::make_shared<BindRenderTargets>()),
		m_clearRenderTarget(std::make_shared<ClearRenderTarget>()),
		m_fullScreenTriangle(std::make_shared<Visual>()),
		m_computerCommand(std::make_shared<ComputerCommand>())
	{
		auto samplerState = std::make_shared<SamplerState>();
		samplerState->load(0);

		auto renderer = Renderer::Create("builtin://renderer/post_processing.hlsl");
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

		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&PostProcessingPipeline::onResize, this, std::placeholders::_1));
	

		m_computerCommand->setDebugName(L"PostComputerCommand");
		m_computerCommand->setComputerEffectPath("builtin://renderer/post_processing.hlsl");
		
	}

	PostProcessingPipeline::~PostProcessingPipeline()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&PostProcessingPipeline::onResize, this, std::placeholders::_1));
	}

	void PostProcessingPipeline::execute(ID3D11DeviceContext* deviceContext)
	{
		m_bindRenderTargets->execute(deviceContext);
		m_clearRenderTarget->execute(deviceContext);

		if (m_renderSystem && m_renderSystem->m_bindRenderTargets)
		{
			if (m_renderSystem->m_bindRenderTargets->getRenderTargetViews(0))
			{
				m_computerCommand->setShaderResourceView("t_texture", m_renderSystem->m_bindRenderTargets->getRenderTargetViews(0)->getTexture());

				//m_fullScreenTriangle->setShaderResource("t_renderedTexture", m_computerCommand->getUnorderedAccessViews(0));
				m_fullScreenTriangle->setShaderResource("t_renderedTexture", m_renderSystem->m_bindRenderTargets->getRenderTargetViews(0)->getTexture());
			}
		}

		m_computerCommand->execute(deviceContext);

		m_fullScreenTriangle->updateDrawParameters();
		m_fullScreenTriangle->execute(deviceContext);
	}

	void PostProcessingPipeline::onResize(void* data)
	{
		WindowResizeData wrd = *(WindowResizeData*)data;
		if (!wrd.width || !wrd.height)
		{
			return;
		}
		//DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
		auto renderTargetView0 = std::make_shared<RenderTargetView>(wrd.width, wrd.height, 2);
		renderTargetView0->load(0);
		
		auto depthStencilView0 = std::make_shared<DepthStencilView>(wrd.width, wrd.height);
		depthStencilView0->load(0);
		
		auto viewPort0 = std::make_shared<D3D11_VIEWPORT>();
		viewPort0->TopLeftX = 0.0f;
		viewPort0->TopLeftY = 0.0f;
		viewPort0->Width = (float)wrd.width;
		viewPort0->Height = (float)wrd.height;
		viewPort0->MinDepth = 0.0f;
		viewPort0->MaxDepth = 1.0f;
		
		std::vector<std::shared_ptr<RenderTargetView>>	renderTargetViews;
		renderTargetViews.push_back(renderTargetView0);

		std::vector<std::shared_ptr<DepthStencilView>>	depthStencilViews;
		depthStencilViews.push_back(depthStencilView0);

		std::vector<std::shared_ptr<D3D11_VIEWPORT>>	viewPorts;
		viewPorts.push_back(viewPort0);

		m_bindRenderTargets->setRenderTargetViews(renderTargetViews);
		m_bindRenderTargets->setDepthStencilViews(depthStencilViews);
		m_bindRenderTargets->setViewports(viewPorts);

		m_clearRenderTarget->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(0));
		m_clearRenderTarget->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(0));
	
		m_computerCommand->setThreadGroupCount((unsigned int)std::ceil(wrd.width / 16), (unsigned int)std::ceil(wrd.height /16), 1);
		auto uav = Texture::Create2DUAV(DXGI_FORMAT_R8G8B8A8_UNORM, wrd.width, wrd.height);
		uav->load(0);
		std::vector<std::shared_ptr<Texture>> uavs = { uav };
		m_computerCommand->setUnorderedAccessViews(uavs);
	}
}