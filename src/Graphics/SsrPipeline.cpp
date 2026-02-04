#include "SsrPipeline.h"
#include "RenderSystem.h"
#include "DeferredOpaquePipeline.h"
#include "BindRenderTargets.h"
#include "RenderTargetView.h"
#include "Visual.h"
#include "Renderer.h"
#include "RenderStates.h"
#include "RenderPass.h"
#include "MeshProvider.h"
#include "Mesh.h"
#include "Texture.h"
#include "SamplerState.h"
#include "Engine/EventSystem.h"
#include "Engine/Engine.h"
#include <d3d11.h>

namespace Destiny
{
	SsrPipeline::SsrPipeline(std::shared_ptr<RenderSystem> renderSystem) :
		GraphicsPipeline(renderSystem),
		m_fullScreenTriangle(std::make_shared<Visual>())
	{
		auto samplerState = std::make_shared<SamplerState>();
		samplerState->load(0);

		auto renderer = Renderer::Create("builtin://renderer/ssr.hlsl");
		renderer->load(0);

		std::shared_ptr<RenderStates> renderStates = std::make_shared<RenderStates>();
		renderStates->getDepthStencilStateDesc()->DepthEnable = false;
		renderStates->getDepthStencilStateDesc()->DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		renderStates->load(0);

		auto mesh = MeshProvider::Create_FullScreenTriangle();
		mesh->load(0);

		auto renderPass = std::make_shared<RenderPass>();
		renderPass->setRenderer(renderer);
		renderPass->setRenderStates(renderStates);

		m_fullScreenTriangle->setMesh(mesh);
		m_fullScreenTriangle->setRenderPass(renderPass);
		m_fullScreenTriangle->setSamplerSate("s_sampler", samplerState);

		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&SsrPipeline::onResize, this, std::placeholders::_1));
	}

	SsrPipeline::~SsrPipeline()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&SsrPipeline::onResize, this, std::placeholders::_1));
	}

	void SsrPipeline::execute(std::shared_ptr<GraphicsContext> deviceContext)
	{
#if 0
		if (!m_renderSystem || !m_renderSystem->m_bindRenderTargets) return;

		auto sceneColorRTV = m_renderSystem->m_bindRenderTargets->getRenderTargetViews(0);
		if (!sceneColorRTV) return;

		if (m_copyTexture && sceneColorRTV->getTexture())
		{
			deviceContext->CopyResource(m_copyTexture->getResource(), sceneColorRTV->getTexture()->getResource());
		}
		else
		{
			return;
		}

		// Bind Render Target (Scene Color)
		m_renderSystem->m_bindRenderTargets->execute(deviceContext);

		// Bind Shader Resources
		m_fullScreenTriangle->setShaderResource("t_sceneColor", m_copyTexture);

		// Get G-Buffer from DeferredOpaquePipeline
		auto deferredPipeline = std::static_pointer_cast<DeferredOpaquePipeline>(m_renderSystem->getDeferredOpaquePipeline());
		if (deferredPipeline && deferredPipeline->m_bindRenderTargets)
		{
			// t_normalW : RT 4
			m_fullScreenTriangle->setShaderResource("t_normalW", deferredPipeline->m_bindRenderTargets->getRenderTargetViews(4)->getTexture());
			// t_positionW : RT 3
			m_fullScreenTriangle->setShaderResource("t_positionW", deferredPipeline->m_bindRenderTargets->getRenderTargetViews(3)->getTexture());
			// t_mra : RT 2
			m_fullScreenTriangle->setShaderResource("t_mra", deferredPipeline->m_bindRenderTargets->getRenderTargetViews(2)->getTexture());
		}

		m_fullScreenTriangle->updateDrawParameters();
		m_fullScreenTriangle->execute(deviceContext);
#endif
	}

	void SsrPipeline::onRendererConstantChanged()
	{
		m_fullScreenTriangle->setRendererConstantChanged();
	}

	void SsrPipeline::onResize(void* data)
	{
		WindowResizeData wrd = *(WindowResizeData*)data;
		if (!wrd.width || !wrd.height) return;

		// Create Copy Texture (Format 2 = R32G32B32A32_FLOAT)
		m_copyTexture = Texture::Create2DSRV(2, wrd.width, wrd.height, nullptr, 0, 0);
		m_copyTexture->load(0);
	}
}
