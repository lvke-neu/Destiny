#include "RenderSystem.h"
#include "Visual.h"
#include "Mesh.h"
#include "RenderPass.h"
#include "ForwardOpaquePipeline.h"
#include "ForwardTransparentPipeline.h"

namespace Destiny
{
	void RenderSystem::createPipeline()
	{
		m_beforePipelineCommandList = std::make_shared<GraphicsCommandList>();

		m_forwardOpaquePipeline = std::make_shared<ForwardOpaquePipeline>();
		m_forwardTransparentPipeline = std::make_shared<ForwardTransparentPipeline>();
	}

	void RenderSystem::render()
	{
		m_beforePipelineCommandList->execute(getImmediateContext());
		m_forwardOpaquePipeline->execute(getImmediateContext());
		m_forwardTransparentPipeline->execute(getImmediateContext());
	}

	void RenderSystem::syncState()
	{
		m_graphicsStat.DrawCallCount = 0;
		m_graphicsStat.TriangleCount = 0;
		m_beforePipelineCommandList->clearGraphicsCommand();
		m_forwardOpaquePipeline->syncState();
		m_forwardTransparentPipeline->syncState();
	}

	void RenderSystem::addBeforePipelineCommand(std::shared_ptr<GraphicsCommand> graphicsCommand)
	{
		m_beforePipelineCommandList->addGraphicsCommand(graphicsCommand);
	}

	void RenderSystem::commitVisual(std::shared_ptr<Visual> visual)
	{
		if (!visual || !visual->getRenderPass())
		{
			return;
		}

		if (visual->getMesh() && visual->getMesh()->getDrawCall().primitiveTopology == Mesh::PrimitiveTopology::TriangleList)
		{
			m_graphicsStat.TriangleCount += visual->getMesh()->getDrawCall().indexCount / 3;
		}

		visual->updateDrawParameters();
		auto renderPass = visual->getRenderPass();
		switch (renderPass->getRendererCategory())
		{
		case RenderPass::ForwardOpaque :
			{
			if (m_forwardOpaquePipeline->addGraphicsCommand(visual))
			{
				++m_graphicsStat.DrawCallCount;
			}
			return;
			}

		case RenderPass::ForwardTransparent :
		{
			if (m_forwardTransparentPipeline->addGraphicsCommand(visual))
			{
				++m_graphicsStat.DrawCallCount;
			}
			return;
		}
		}
	}
}