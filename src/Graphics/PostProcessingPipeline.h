#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny 
{
	class BindRenderTargets;
	class ClearRenderTarget;
	class Visual;
	class ComputerCommand;
	class PostProcessingPipeline : public GraphicsPipeline
	{
	public:
		PostProcessingPipeline(std::shared_ptr<RenderSystem> renderSystem);
		virtual ~PostProcessingPipeline();
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	public:
		std::shared_ptr<BindRenderTargets>		m_bindRenderTargets;
	private:
		void onResize(void* data);
	private:
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget;
		std::shared_ptr<Visual>					m_fullScreenTriangle;
		std::shared_ptr<ComputerCommand>		m_computerCommand;
	};
}