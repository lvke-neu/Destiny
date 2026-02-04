#include "ShadowMapPipeline.h"
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
	ShadowMapPipeline::ShadowMapPipeline(std::shared_ptr<RenderSystem> renderSystem) :
		GraphicsPipeline(renderSystem),
		m_bindRenderTargets(std::make_shared<BindRenderTargets>()),
		m_clearRenderTarget(std::make_shared<ClearRenderTarget>())
	{
		m_shadowMapSampler = std::make_shared<SamplerState>();

		m_shadowMapSampler->getSamplerDesc()->Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		m_shadowMapSampler->getSamplerDesc()->AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		m_shadowMapSampler->getSamplerDesc()->AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		m_shadowMapSampler->getSamplerDesc()->AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		m_shadowMapSampler->getSamplerDesc()->ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		m_shadowMapSampler->getSamplerDesc()->BorderColor[0] = { 1.0f };
		m_shadowMapSampler->getSamplerDesc()->MinLOD = 0;
		m_shadowMapSampler->getSamplerDesc()->MaxLOD = D3D11_FLOAT32_MAX;
		m_shadowMapSampler->getSamplerDesc()->MaxAnisotropy = 1;
		m_shadowMapSampler->load(0);
	}

	ShadowMapPipeline::~ShadowMapPipeline()
	{
		
	}

	void ShadowMapPipeline::execute(std::shared_ptr<GraphicsContext> deviceContext)
	{
#if 0
		m_bindRenderTargets->execute(deviceContext);
		m_clearRenderTarget->execute(deviceContext);

		GraphicsCommandList::execute(deviceContext);
#endif
	}

	void ShadowMapPipeline::onResize(float viewPortWidth, float viewPortHeight)
	{
		auto renderTargetView = std::make_shared<RenderTargetView>((unsigned int)viewPortWidth, (unsigned int)viewPortHeight);
		renderTargetView->load(0);
		renderTargetView->setDebugObjectName("ShadowMapPipelineRTV");

		auto depthStencilView = std::make_shared<DepthStencilView>((unsigned int)viewPortWidth, (unsigned int)viewPortHeight);
		depthStencilView->load(0);

		auto viewPort = std::make_shared<D3D11_VIEWPORT>();
		viewPort->TopLeftX = 0.0f;
		viewPort->TopLeftY = 0.0f;
		viewPort->Width = viewPortWidth;
		viewPort->Height = viewPortHeight;
		viewPort->MinDepth = 0.0f;
		viewPort->MaxDepth = 1.0f;

		std::vector<std::shared_ptr<RenderTargetView>>	renderTargetViews;
		renderTargetViews.push_back(renderTargetView);

		std::vector<std::shared_ptr<DepthStencilView>>	depthStencilViews;
		depthStencilViews.push_back(depthStencilView);

		std::vector<std::shared_ptr<D3D11_VIEWPORT>>	viewPorts;
		viewPorts.push_back(viewPort);

		m_bindRenderTargets->setRenderTargetViews(renderTargetViews);
		m_bindRenderTargets->setDepthStencilViews(depthStencilViews);
		m_bindRenderTargets->setViewports(viewPorts);

		m_clearRenderTarget->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(0));
		m_clearRenderTarget->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(0));
	
		Renderer::SetShaderResource("t_shadowMap", m_bindRenderTargets->getDepthStencilViews(0)->getTexture());
		Renderer::SetSamplerSate("s_shadowMapSampler", m_shadowMapSampler);
	}
}