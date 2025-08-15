#include "RenderSystem.h"
#include "Visual.h"
#include "Mesh.h"
#include "RenderPass.h"
#include "ShadowMapPipeline.h"
#include "DeferredOpaquePipeline.h"
#include "ForwardOpaquePipeline.h"
#include "TransparentPipeline.h"
#include "GuiPipeline.h"
#include "PostProcessingPipeline.h"
#include "BindRenderTargets.h"
#include "ClearRenderTarget.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "GpuTimer.h"
#include "Engine/EventSystem.h"

namespace Destiny
{
	RenderSystem::RenderSystem() :
		m_shadowMapPipeline(nullptr),
		m_deferredOpaquePipeline(nullptr),
		m_forwardOpaquePipeline(nullptr),
		m_transparentPipeline(nullptr),
		m_guiPipeline(nullptr),
		m_postProcessingPipeline(nullptr),
		m_commonRenderTargetCommandList(std::make_shared<GraphicsCommandList>()),
		m_beforePipelineCommandList(std::make_shared<GraphicsCommandList>()),
		m_bindRenderTargets(std::make_shared<BindRenderTargets>()),
		m_clearRenderTarget(std::make_shared<ClearRenderTarget>()),
		m_gpuTimer(std::make_shared<GpuTimer>())
	{
		m_commonRenderTargetCommandList->addGraphicsCommand(m_bindRenderTargets);
		m_commonRenderTargetCommandList->addGraphicsCommand(m_clearRenderTarget);
	}

	RenderSystem::~RenderSystem()
	{
		Engine::GetInstance()->getEventSystem()->unRegisterEvent(EventType::WindowResize, std::bind(&RenderSystem::onResize, this, std::placeholders::_1));
	}

	void RenderSystem::createPipeline()
	{
		Engine::GetInstance()->getEventSystem()->registerEvent(EventType::WindowResize, std::bind(&RenderSystem::onResize, this, std::placeholders::_1));

		m_shadowMapPipeline = std::make_shared<ShadowMapPipeline>(shared_from_this());
		m_deferredOpaquePipeline = std::make_shared<DeferredOpaquePipeline>(shared_from_this());
		m_forwardOpaquePipeline = std::make_shared<ForwardOpaquePipeline>(shared_from_this());
		m_transparentPipeline = std::make_shared<TransparentPipeline>(shared_from_this());
		m_guiPipeline = std::make_shared<GuiPipeline>(shared_from_this());
		m_postProcessingPipeline = std::make_shared<PostProcessingPipeline>(shared_from_this());

		m_gpuTimer->Init(getDevice(), getImmediateContext());
	}

	void RenderSystem::render()
	{
		auto deviceContext = getImmediateContext();

		beginEvent(L"Standard Scene");

		beginEvent(L"Before Pipeline");
		m_beforePipelineCommandList->execute(deviceContext);
		for (const auto& commandList : m_beforePipelineCommandLists)
		{
			beginEvent(commandList.first.c_str());
			if (commandList.second)
			{
				commandList.second->execute(deviceContext);
			}
			endEvent();
		}
		endEvent();

		beginEvent(L"Common RenderTarget");
		m_commonRenderTargetCommandList->execute(deviceContext);
		endEvent();

		beginEvent(L"Shadow Pass");
		m_shadowMapPipeline->execute(deviceContext);
		endEvent();

		beginEvent(L"DeferredOpaque Pass");
		m_deferredOpaquePipeline->execute(deviceContext);
		endEvent();
 
		beginEvent(L"ForwardOpaque Pass");
		m_forwardOpaquePipeline->execute(deviceContext);
		endEvent();

		beginEvent(L"Transparent Pass");
		m_transparentPipeline->execute(deviceContext);
		endEvent();

		beginEvent(L"Gui Pass");
		m_guiPipeline->execute(deviceContext);
		endEvent();

		beginEvent(L"Postprocess Pass");
		m_postProcessingPipeline->execute(deviceContext);
		endEvent();

		endEvent();
	}

	void RenderSystem::syncState()
	{
		m_graphicsStat.DrawCallCount = 0;
		m_graphicsStat.TriangleCount = 0;
		m_graphicsStat.VisualCount = 0;
		m_graphicsStat.DeferredVisualCount = 0;
		m_graphicsStat.ForwardVisualCount = 0;
		m_graphicsStat.TransparentVisualCount = 0;
		m_graphicsStat.GuiVisualCount = 0;

		m_shadowMapPipeline->syncState();
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

		if (visual->get_visualCategory() == VisualCategory::RenderToShadowMap && visual->get_enableShadow())
		{
			visual->updateDrawParameters();
			if (m_shadowMapPipeline->addGraphicsCommand(visual))
			{
				++m_graphicsStat.DrawCallCount;
				++m_graphicsStat.VisualCount;
				if (visual->getMesh() && visual->getMesh()->getDrawCall().primitiveTopology == Mesh::PrimitiveTopology::TriangleList)
				{
					m_graphicsStat.TriangleCount += visual->getMesh()->getDrawCall().indexCount / 3;
				}
			}
		}
		else if (visual->get_visualCategory() == VisualCategory::RenderToScene)
		{
			visual->updateDrawParameters();
			auto renderPass = visual->getRenderPass();
			switch (renderPass->getRendererCategory())
			{
			case RendererCategory::DeferredOpaque:
			{
				if (m_deferredOpaquePipeline->addGraphicsCommand(visual))
				{
					++m_graphicsStat.DrawCallCount;
					++m_graphicsStat.VisualCount;
					++m_graphicsStat.DeferredVisualCount;
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
					++m_graphicsStat.ForwardVisualCount;
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
					++m_graphicsStat.TransparentVisualCount;
					if (visual->getMesh() && visual->getMesh()->getDrawCall().primitiveTopology == Mesh::PrimitiveTopology::TriangleList)
					{
						m_graphicsStat.TriangleCount += visual->getMesh()->getDrawCall().indexCount / 3;
					}
				}
				return;
			}
			case RendererCategory::Gui:
			{
				if (m_guiPipeline->addGraphicsCommand(visual))
				{
					++m_graphicsStat.DrawCallCount;
					++m_graphicsStat.VisualCount;
					++m_graphicsStat.GuiVisualCount;
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

	std::shared_ptr<RenderTargetView> RenderSystem::getRenderTargetView()
	{
		//return m_bindRenderTargets->getRenderTargetViews(0);
		return std::static_pointer_cast<PostProcessingPipeline>(m_postProcessingPipeline)->m_bindRenderTargets->getRenderTargetViews(0);
	}

	void RenderSystem::addBeforePipelineCommand(std::shared_ptr<GraphicsCommand> graphicsCommand)
	{
		m_beforePipelineCommandList->addGraphicsCommand(graphicsCommand);
	}

	void RenderSystem::clearBeforePipelineCommand()
	{
		m_beforePipelineCommandList->clearGraphicsCommand();
	}

	void RenderSystem::addBeforePipelineCommandList(const std::wstring& debugName, std::shared_ptr<GraphicsCommandList> graphicsCommandList)
	{
		m_beforePipelineCommandLists.push_back({ debugName,  graphicsCommandList });
	}

	void RenderSystem::clear()
	{
		m_beforePipelineCommandLists.clear();
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