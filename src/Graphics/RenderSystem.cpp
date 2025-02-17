#include "RenderSystem.h"
#include "Visual.h"
#include "Mesh.h"
#include "RenderPass.h"
#include "ForwardOpaquePipeline.h"
#include "ForwardTransparentPipeline.h"
#include "GuiPipeline.h"
#include "PostProcessPipeline.h"

namespace Destiny
{
	void RenderSystem::createPipeline()
	{
		m_forwardOpaquePipeline = std::make_shared<ForwardOpaquePipeline>();
		m_transparentPipeline = std::make_shared<ForwardTransparentPipeline>();
		m_guiPipeline = std::make_shared<GuiPipeline>();
		m_postProcessPipeline = std::make_shared<PostProcessPipeline>();
	}

	void RenderSystem::render()
	{
		m_forwardOpaquePipeline->execute(getImmediateContext());
		m_transparentPipeline->execute(getImmediateContext());
		m_guiPipeline->execute(getImmediateContext());
		m_postProcessPipeline->execute(getImmediateContext());
	}

	void RenderSystem::syncState()
	{
		m_graphicsStat.DrawCallCount = 0;
		m_graphicsStat.TriangleCount = 0;
		m_graphicsStat.VisualCount = 0;

		m_forwardOpaquePipeline->syncState();
		m_transparentPipeline->syncState();
		m_guiPipeline->syncState();
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

		case RendererCategory::Transparent:
		{
			if (m_transparentPipeline->addGraphicsCommand(visual))
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