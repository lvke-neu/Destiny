#include "RenderSystem.h"
#include "Visual.h"
#include "Mesh.h"
#include "RenderPass.h"
#include "DeferredOpaquePipeline.h"
#include "ForwardOpaquePipeline.h"
#include "TransparentPipeline.h"
#include "GuiPipeline.h"
#include "PostProcessingPipeline.h"
#include "BindRenderTargets.h"
#include "ClearRenderTarget.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Engine/EventSystem.h"

namespace Destiny
{
	RenderSystem::RenderSystem() :
		m_deferredOpaquePipeline(nullptr),
		m_forwardOpaquePipeline(nullptr),
		m_transparentPipeline(nullptr),
		m_guiPipeline(nullptr),
		m_postProcessingPipeline(nullptr),
		m_beforePipelineCommand(std::make_shared<GraphicsCommandList>()),
		m_bindRenderTargets(std::make_shared<BindRenderTargets>()),
		m_clearRenderTarget(std::make_shared<ClearRenderTarget>())
	{
		m_beforePipelineCommand->addGraphicsCommand(m_bindRenderTargets);
		m_beforePipelineCommand->addGraphicsCommand(m_clearRenderTarget);
	}

	RenderSystem::~RenderSystem()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&RenderSystem::onResize, this, std::placeholders::_1));
	}

	void RenderSystem::createPipeline()
	{
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&RenderSystem::onResize, this, std::placeholders::_1));

		m_deferredOpaquePipeline = std::make_shared<DeferredOpaquePipeline>(shared_from_this());
		m_forwardOpaquePipeline = std::make_shared<ForwardOpaquePipeline>(shared_from_this());
		m_transparentPipeline = std::make_shared<TransparentPipeline>(shared_from_this());
		m_guiPipeline = std::make_shared<GuiPipeline>(shared_from_this());
		m_postProcessingPipeline = std::make_shared<PostProcessingPipeline>(shared_from_this());
	}

	void RenderSystem::render()
	{
		m_beforePipelineCommand->execute(getImmediateContext());
		m_deferredOpaquePipeline->execute(getImmediateContext());
		m_forwardOpaquePipeline->execute(getImmediateContext());
		m_transparentPipeline->execute(getImmediateContext());
		m_guiPipeline->execute(getImmediateContext());
		m_postProcessingPipeline->execute(getImmediateContext());
	}

	void RenderSystem::syncState()
	{
		m_graphicsStat.DrawCallCount = 0;
		m_graphicsStat.TriangleCount = 0;
		m_graphicsStat.VisualCount = 0;

		m_deferredOpaquePipeline->syncState();
		m_forwardOpaquePipeline->syncState();
		m_transparentPipeline->syncState();
		m_guiPipeline->syncState();
		m_postProcessingPipeline->syncState();
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
		case RendererCategory::DeferredOpaque :
		{
			if (m_deferredOpaquePipeline->addGraphicsCommand(visual))
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
		case RendererCategory::ForwardOpaque:
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

	std::shared_ptr<RenderTargetView> RenderSystem::getRenderTargetView()
	{
		//return m_bindRenderTargets->getRenderTargetViews(0);
		return std::static_pointer_cast<PostProcessingPipeline>(m_postProcessingPipeline)->m_bindRenderTargets->getRenderTargetViews(0);
	}

	void RenderSystem::onResize(void* data)
	{
		WindowResizeData wrd = *(WindowResizeData*)data;
		if (!wrd.width || !wrd.height)
		{
			return;
		}

		auto renderTargetView = std::make_shared<RenderTargetView>(wrd.width, wrd.height);
		renderTargetView->load(0);

		auto depthStencilView = std::make_shared<DepthStencilView>(wrd.width, wrd.height);
		depthStencilView->load(0);

		auto viewPort = std::make_shared<D3D11_VIEWPORT>();
		viewPort->TopLeftX = 0.0f;
		viewPort->TopLeftY = 0.0f;
		viewPort->Width = (float)wrd.width;
		viewPort->Height = (float)wrd.height;
		viewPort->MinDepth = 0.0f;
		viewPort->MaxDepth = 1.0f;

		std::vector<std::shared_ptr<RenderTargetView>>	renderTargetViews;
		std::vector<std::shared_ptr<DepthStencilView>>	depthStencilViews;
		std::vector<std::shared_ptr<D3D11_VIEWPORT>>	viewPorts;
		renderTargetViews.push_back(renderTargetView);
		depthStencilViews.push_back(depthStencilView);
		viewPorts.push_back(viewPort);

		m_bindRenderTargets->setRenderTargetViews(renderTargetViews);
		m_bindRenderTargets->setDepthStencilViews(depthStencilViews);
		m_bindRenderTargets->setViewports(viewPorts);

		m_clearRenderTarget->setRenderTargetView(m_bindRenderTargets->getRenderTargetViews(0));
		m_clearRenderTarget->setDepthStencilView(m_bindRenderTargets->getDepthStencilViews(0));
	}
}