#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny 
{
	class BindRenderTargets;
	class ClearRenderTarget;
	class DrawCommand;
	class DeferredOpaquePipeline : public GraphicsPipeline
	{
	public:
		DeferredOpaquePipeline(std::shared_ptr<RenderSystem> renderSystem);
		virtual ~DeferredOpaquePipeline();
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	private:
		void onResize(void* data);
	private:
		std::shared_ptr<BindRenderTargets>		m_bindRenderTargets;
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget0;
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget1;
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget2;
		std::shared_ptr<DrawCommand>			m_fullScreenTriangle;
	};
}