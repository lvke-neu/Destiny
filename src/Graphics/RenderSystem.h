#pragma once
#include "GraphicsSystem.h"
#include "GraphicsPipeline/GraphicsPipeline.h"
#include <unordered_map>

class GpuTimer;
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
		void									addBeforePipelineCommand(std::shared_ptr<GraphicsCommand> graphicsCommand);
		void									removeBeforePipelineCommand(std::shared_ptr<GraphicsCommand> graphicsCommand);
		void									addBeforePipelineCommandList(const std::wstring& debugName, std::shared_ptr<GraphicsCommandList> graphicsCommandList);
		void									removeBeforePipelineCommandList(const std::wstring& debugName, std::shared_ptr<GraphicsCommandList> graphicsCommandList);
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

		std::vector<std::pair<std::wstring, std::shared_ptr<GraphicsCommandList>>> m_beforePipelineCommandLists;
		std::shared_ptr<GraphicsCommandList>	m_beforePipelineCommandList;
		std::shared_ptr<GraphicsCommandList>	m_commonRenderTargetCommandList;
		std::shared_ptr<ClearRenderTarget>		m_clearRenderTarget;

		std::shared_ptr<GpuTimer>				m_gpuTimer;
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