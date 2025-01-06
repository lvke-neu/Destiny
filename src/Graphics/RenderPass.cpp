#include "RenderPass.h"
#include "RenderStates.h"

namespace Destiny
{
	RenderPass::RenderPass() :
		m_renderer(nullptr),
		m_renderStates(nullptr),
		m_rendererCategory(RendererCategory::None)
	{

	}

	std::string RenderPass::get_renderer()
	{
		return m_renderer ? m_renderer->getPath() : "";
	}

	void RenderPass::set_renderer(std::string renderer)
	{
		auto rdr = std::make_shared<Renderer>(renderer.c_str());
		rdr->load(0);
		if (rdr->isLoadingSucceed())
		{
			setRenderer(rdr);
		}
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
		if (!renderer)
		{
			return;
		}

		//renderer->copy_constant_texture_sampler(m_renderer);
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

	void RenderPass::load(int priority)
	{
		if (m_renderer)
		{
			m_renderer->load(priority);
		}
		if (m_renderStates)
		{
			m_renderStates->load(priority);
		}
	}
}
