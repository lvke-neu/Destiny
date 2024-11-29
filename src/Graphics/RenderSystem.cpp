#include "RenderSystem.h"
#include "ForwardOpaquePipeline.h"
#include "ForwardTransparentPipeline.h"
#include "Visual.h"
#include "RenderPass.h"
#include "RenderCommandList.h"
#include "BindRenderTargetsOnResize.h"

namespace Destiny
{
	void RenderSystem::createPipeline()
	{
		m_forwardOpaquePipeline = std::make_shared<ForwardOpaquePipeline>();
		m_forwardTransparentPipeline = std::make_shared<ForwardTransparentPipeline>();

		m_beforeForwardOpaqueCommandList = std::make_shared<RenderCommandList>();
		m_beforeForwardTransparentCommandList = std::make_shared<RenderCommandList>();

		bindRenderTargetsOnResize = std::make_shared<BindRenderTargetsOnResize>();
		addBeforeForwardOpaqueCommand(bindRenderTargetsOnResize);
		addBeforeForwardTransparentCommandList(std::make_shared<BindRenderTargetsOnResize>());
	}

	void RenderSystem::render()
	{
		m_beforeForwardOpaqueCommandList->execute(getImmediateContext());
		m_forwardOpaquePipeline->execute(getImmediateContext());
		m_beforeForwardTransparentCommandList->execute(getImmediateContext());
		m_forwardTransparentPipeline->execute(getImmediateContext());
	}

	void RenderSystem::syncState()
	{
		m_forwardOpaquePipeline->syncState();
		m_forwardTransparentPipeline->syncState();
	}

	void RenderSystem::commitVisual(std::shared_ptr<Visual> visual)
	{
		if (!visual || !visual->getRenderPass())
		{
			return;
		}

		visual->upDrawParameters();
		auto renderPass = visual->getRenderPass();
		switch (renderPass->getPipeline())
		{
		case RenderPass::ForwardOpaque :
			{
			m_forwardOpaquePipeline->addRenderCommand(visual);
			return;
			}

		case RenderPass::ForwardTransparent :
		{
			m_forwardTransparentPipeline->addRenderCommand(visual);
			return;
		}
		}
	}

	void RenderSystem::addBeforeForwardOpaqueCommand(std::shared_ptr<RenderCommand> renderCommand)
	{
		m_beforeForwardOpaqueCommandList->addRenderCommand(renderCommand);
	}

	void RenderSystem::addBeforeForwardTransparentCommandList(std::shared_ptr<RenderCommand> renderCommand)
	{
		m_beforeForwardTransparentCommandList->addRenderCommand(renderCommand);
	}
}