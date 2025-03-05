#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny 
{
	class BindRenderTargets;
	class ClearRenderTarget;
	class Visual;
	class DeferredOpaquePipeline : public GraphicsPipeline
	{
	public:
		DeferredOpaquePipeline(std::shared_ptr<RenderSystem> renderSystem);
		virtual ~DeferredOpaquePipeline();
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	public:
		std::shared_ptr<BindRenderTargets>		m_bindRenderTargets;
	private:
		void onResize(void* data);
	private:
		//albedo
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget0;
		//normal
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget1;
		//mra
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget2;
		std::shared_ptr<Visual>					m_fullScreenTriangle;
	};
}