#include "RenderSystem.h"
#include "Visual.h"
#include "Mesh.h"
#include "RenderPass.h"
#include "ForwardOpaquePipeline.h"
#include "ForwardTransparentPipeline.h"
#include "GuiPipeline.h"

namespace Destiny
{
	void RenderSystem::createPipeline()
	{
		m_beforePipelineCommandList = std::make_shared<GraphicsCommandList>();

		m_forwardOpaquePipeline = std::make_shared<ForwardOpaquePipeline>();
		m_forwardTransparentPipeline = std::make_shared<ForwardTransparentPipeline>();
		m_guiPipeline = std::make_shared<GuiPipeline>();
	}

	void RenderSystem::render()
	{
		m_beforePipelineCommandList->execute(getImmediateContext());
		m_forwardOpaquePipeline->execute(getImmediateContext());
		m_forwardTransparentPipeline->execute(getImmediateContext());
		m_guiPipeline->execute(getImmediateContext());
	}

	void RenderSystem::syncState()
	{
		m_graphicsStat.DrawCallCount = 0;
		m_graphicsStat.TriangleCount = 0;
		m_graphicsStat.VisualCount = 0;
		m_beforePipelineCommandList->clearGraphicsCommand();
		m_forwardOpaquePipeline->syncState();
		m_forwardTransparentPipeline->syncState();
		m_guiPipeline->syncState();
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

		visual->updateDrawParameters();
		auto renderPass = visual->getRenderPass();
		switch (renderPass->getRendererCategory())
		{
		case RendererCategory::ForwardOpaque :
		{
			if (m_forwardOpaquePipeline->addGraphicsCommand(visual))
			{
				++m_graphicsStat.DrawCallCount;
				++m_graphicsStat.VisualCount;
				if (visual->getMesh() && visual->getMesh()->getDrawCall().primitiveTopology == Mesh::PrimitiveTopology::TriangleList)
				{
					m_graphicsStat.TriangleCount += visual->getMesh()->getDrawCall().indexCount / 3;
				}
			}
			return;
			}

		case RendererCategory::ForwardTransparent :
		{
			if (m_forwardTransparentPipeline->addGraphicsCommand(visual))
			{
				++m_graphicsStat.DrawCallCount;
				++m_graphicsStat.VisualCount;
				if (visual->getMesh() && visual->getMesh()->getDrawCall().primitiveTopology == Mesh::PrimitiveTopology::TriangleList)
				{
					m_graphicsStat.TriangleCount += visual->getMesh()->getDrawCall().indexCount / 3;
				}
			}
			return;
		}
		case RendererCategory::Gui :
		{
			if (m_guiPipeline->addGraphicsCommand(visual))
			{
				++m_graphicsStat.DrawCallCount;
				++m_graphicsStat.VisualCount;
				if (visual->getMesh() && visual->getMesh()->getDrawCall().primitiveTopology == Mesh::PrimitiveTopology::TriangleList)
				{
					m_graphicsStat.TriangleCount += visual->getMesh()->getDrawCall().indexCount / 3;
				}
			}
			return;
		}
		}
	}
}