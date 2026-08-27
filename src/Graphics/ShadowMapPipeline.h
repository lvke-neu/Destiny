#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny 
{
	class BindRenderTargets;
	class ClearRenderTarget;
	class SamplerState;
	class ShadowMapPipeline : public GraphicsPipeline
	{
	public:
		ShadowMapPipeline(std::shared_ptr<RenderSystem> renderSystem);
		virtual ~ShadowMapPipeline();
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	public:
		void onResize(float viewPortWidth, float viewPortHeight);
	public:
		std::shared_ptr<BindRenderTargets>		m_bindRenderTargets;
	private:
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget;
		std::shared_ptr<SamplerState>			m_shadowMapSampler;
	};
}