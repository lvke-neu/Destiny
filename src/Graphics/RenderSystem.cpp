#include "RenderSystem.h"
#include "Visual.h"
#include "Mesh.h"
#include "RenderPass.h"
#include "ForwardOpaquePipeline.h"
#include "TransparentPipeline.h"
#include "GuiPipeline.h"
#include "BindRenderTargets.h"
#include "ClearRenderTargets.h"
#include "Engine/EventSystem.h"

namespace Destiny
{
	RenderSystem::RenderSystem() :
		m_forwardOpaquePipeline(nullptr),
		m_transparentPipeline(nullptr),
		m_guiPipeline(nullptr),
		m_beforePipelineCommand(std::make_shared<GraphicsCommandList>()),
		m_bindRenderTargets(std::make_shared<BindRenderTargets>()),
		m_clearRenderTargets(std::make_shared<ClearRenderTargets>())
	{
		m_beforePipelineCommand->addGraphicsCommand(m_clearRenderTargets);
	}

	RenderSystem::~RenderSystem()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&RenderSystem::onResize, this, std::placeholders::_1));
	}

	void RenderSystem::createPipeline()
	{
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&RenderSystem::onResize, this, std::placeholders::_1));

		m_forwardOpaquePipeline = std::make_shared<ForwardOpaquePipeline>(shared_from_this());
		m_transparentPipeline = std::make_shared<TransparentPipeline>(shared_from_this());
		m_guiPipeline = std::make_shared<GuiPipeline>(shared_from_this());
	}

	void RenderSystem::render()
	{
		m_beforePipelineCommand->execute(getImmediateContext());
		m_forwardOpaquePipeline->execute(getImmediateContext());
		m_transparentPipeline->execute(getImmediateContext());
		m_guiPipeline->execute(getImmediateContext());
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

	void RenderSystem::onResize(void* data)
	{
		WindowResizeData wrd = *(WindowResizeData*)data;
		if (!wrd.width || !wrd.height)
		{
			return;
		}

		m_bindRenderTargets->setRenderTargetView(wrd.width, wrd.height);
		m_bindRenderTargets->setDepthStencilView(wrd.width, wrd.height);
		m_bindRenderTargets->setViewport(0.0f, 0.0f, (float)wrd.width, (float)wrd.height, 0.0f, 0.0f);

		m_clearRenderTargets->setRenderTargetView(m_bindRenderTargets->getRenderTargetView());
		m_clearRenderTargets->setDepthStencilView(m_bindRenderTargets->getDepthStencilView());
	}
}