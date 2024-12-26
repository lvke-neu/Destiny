#include "RenderSystem.h"
#include "Visual.h"
#include "Mesh.h"
#include "RenderPass.h"
#include "BindRenderTargetsOnResize.h"
#include "ForwardOpaquePipeline.h"
#include "ForwardTransparentPipeline.h"
#include "GraphicsPipeline/GraphicsCommandList.h"


namespace Destiny
{
	void RenderSystem::createPipeline()
	{
		m_forwardOpaquePipeline = std::make_shared<ForwardOpaquePipeline>();
		m_forwardTransparentPipeline = std::make_shared<ForwardTransparentPipeline>();

		m_beforeForwardOpaqueCommandList = std::make_shared<GraphicsCommandList>();
		m_beforeForwardTransparentCommandList = std::make_shared<GraphicsCommandList>();

		bindRenderTargetsOnResize = std::make_shared<BindRenderTargetsOnResize>();
		addBeforeForwardOpaqueCommand(bindRenderTargetsOnResize);
		//addBeforeForwardTransparentCommandList(std::make_shared<BindRenderTargetsOnResize>());
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
		m_graphicsStat.DrawCallCount = 0;
		m_graphicsStat.TriangleCount = 0;
		m_forwardOpaquePipeline->syncState();
		m_forwardTransparentPipeline->syncState();
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

	void RenderSystem::addBeforeForwardOpaqueCommand(std::shared_ptr<GraphicsCommand> graphicsCommand)
	{
		m_beforeForwardOpaqueCommandList->addGraphicsCommand(graphicsCommand);
	}

	void RenderSystem::addBeforeForwardTransparentCommandList(std::shared_ptr<GraphicsCommand> graphicsCommand)
	{
		m_beforeForwardTransparentCommandList->addGraphicsCommand(graphicsCommand);
	}
}