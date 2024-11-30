#include "RenderPass.h"
#include "Renderer.h"
#include "RenderStates.h"

namespace Destiny
{
	RenderPass::RenderPass() :
		m_renderer(nullptr),
		m_renderStates(nullptr),
		m_rendererCategory(None)
	{

	}

	void RenderPass::setRenderer(std::shared_ptr<Renderer> renderer)
	{
		//if (!renderer)
		//{
		//	return;
		//}

		//if (renderer->isLoadingPending())
		//{
		//	renderer->load(0);
		//}

		//if (!renderer->isLoadingSucceed())
		//{
		//	return;
		//}

		m_renderer.reset();
		m_renderer = renderer;
	}

	void RenderPass::setRenderStates(std::shared_ptr<RenderStates> renderStates)
	{
		if (!renderStates)
		{
			return;
		}

		if (renderStates->isLoadingPending())
		{
			renderStates->load(0);
		}

		if (!renderStates->isLoadingSucceed())
		{
			return;
		}

		m_renderStates.reset();
		m_renderStates = renderStates;
	}

	void RenderPass::fillDrawParameters(std::shared_ptr<DrawParameters> drawParameters)
	{
		if (m_renderer)
		{
			m_renderer->fillDrawParameters(drawParameters);
		}

		if (m_renderStates)
		{
			m_renderStates->fillDrawParameters(drawParameters);
		}
	}
}
