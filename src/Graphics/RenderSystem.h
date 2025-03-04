#pragma once
#include "GraphicsSystem.h"
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny
{
	class RenderTargetView;
	class BindRenderTargets;
	class ClearRenderTargets;
	class Visual;
	class RenderSystem : public GraphicsSystem, public std::enable_shared_from_this<RenderSystem>
	{
	public:
		RenderSystem();
		virtual ~RenderSystem();
	public:
		virtual void							createPipeline() override;
		virtual void							render() override;
		virtual void							syncState() override;
		void									commitVisual(std::shared_ptr<Visual> visual);
	public:
		std::shared_ptr<BindRenderTargets>		m_bindRenderTargets;
	private:
		void onResize(void* data);
	private:
		std::shared_ptr<GraphicsPipeline>		m_forwardOpaquePipeline;
		std::shared_ptr<GraphicsPipeline>		m_transparentPipeline;
		std::shared_ptr<GraphicsPipeline>		m_guiPipeline;

		std::shared_ptr<GraphicsCommandList>	m_beforePipelineCommand;
		std::shared_ptr<ClearRenderTargets>		m_clearRenderTargets;
	};
}