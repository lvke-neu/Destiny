#include "RenderSystem.h"
#include "ForwardOpaquePipeline.h"
#include "ForwardTransparentPipeline.h"
#include "Visual.h"
#include "RenderPass.h"

namespace Destiny
{
	void RenderSystem::createPipeline()
	{
		m_forwardOpaquePipeline = std::make_shared<ForwardOpaquePipeline>();
		m_forwardTransparentPipeline = std::make_shared<ForwardTransparentPipeline>();
	}

	void RenderSystem::render()
	{
		m_forwardOpaquePipeline->execute(getImmediateContext());
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
}