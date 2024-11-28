#pragma once
#include "GraphicsSystem.h"
#include <memory>

namespace Destiny
{
	class RenderCommand;
	class RenderCommandList;
	class ForwardOpaquePipeline;
	class ForwardTransparentPipeline;
	class BindRenderTargetsOnResize;
	class RenderSystem : public GraphicsSystem
	{
	public:
		virtual ~RenderSystem() = default;
	public:
		virtual void createPipeline() override;
		virtual void render() override;
		virtual void syncState() override;
		virtual void commitVisual(std::shared_ptr<Visual> visual) override;
	public:
		void addBeforeForwardOpaqueCommand(std::shared_ptr<RenderCommand> renderCommand);
	public:
		std::shared_ptr<BindRenderTargetsOnResize> bindRenderTargetsOnResize;
	private:
		std::shared_ptr<ForwardOpaquePipeline> m_forwardOpaquePipeline;
		std::shared_ptr<ForwardTransparentPipeline> m_forwardTransparentPipeline;
		
		std::shared_ptr<RenderCommandList> m_beforeForwardOpaqueCommandList;
	};

}