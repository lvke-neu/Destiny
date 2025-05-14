#pragma once
#include "GraphicsSystem.h"
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny
{
	class RenderTargetView;
	class BindRenderTargets;
	class ClearRenderTarget;
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
		std::shared_ptr<GraphicsPipeline>       getDeferredOpaquePipeline();
		std::shared_ptr<GraphicsPipeline>       getForwardOpaquePipeline();
		std::shared_ptr<GraphicsPipeline>		getShadowMapPipeline();
		std::shared_ptr<RenderTargetView>		getRenderTargetView();
	public:
		std::shared_ptr<BindRenderTargets>		m_bindRenderTargets;
	private:
		void onResize(void* data);
	private:
		std::shared_ptr<GraphicsPipeline>		m_shadowMapPipeline;
		std::shared_ptr<GraphicsPipeline>		m_forwardOpaquePipeline;
		std::shared_ptr<GraphicsPipeline>		m_deferredOpaquePipeline;
		std::shared_ptr<GraphicsPipeline>		m_transparentPipeline;
		std::shared_ptr<GraphicsPipeline>		m_guiPipeline;
		std::shared_ptr<GraphicsPipeline>		m_postProcessingPipeline;

		std::shared_ptr<GraphicsCommandList>	m_beforePipelineCommand;
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget;
	};

	inline std::shared_ptr<GraphicsPipeline> RenderSystem::getDeferredOpaquePipeline()
	{
		return m_deferredOpaquePipeline;
	}

	inline std::shared_ptr<GraphicsPipeline> RenderSystem::getForwardOpaquePipeline()
	{
		return m_forwardOpaquePipeline;
	}

	inline std::shared_ptr<GraphicsPipeline> RenderSystem::getShadowMapPipeline()
	{
		return m_shadowMapPipeline;
	}
}