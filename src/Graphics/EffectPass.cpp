#include "EffectPass.h"
#include "Renderer.h"
#include "RenderStates.h"

namespace Destiny
{
	EffectPass::EffectPass() : 
		m_renderer(nullptr),
		m_renderStates(nullptr)
	{

	}

	void EffectPass::setRenderer(std::shared_ptr<Renderer> renderer)
	{
		if (!renderer)
		{
			return;
		}

		if (renderer->isLoadingPending())
		{
			renderer->load(0);
		}

		if (!renderer->isLoadingSucceed())
		{
			return;
		}

		m_renderer.reset();
		m_renderer = renderer;
	}

	void EffectPass::setRenderStates(std::shared_ptr<RenderStates> renderStates)
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

	void EffectPass::fillRenderParameters(std::shared_ptr<RenderParameters> renderParameters)
	{
		if (m_renderer)
		{
			m_renderer->fillRenderParameters(renderParameters);
		}

		if (m_renderStates)
		{
			m_renderStates->fillRenderParameters(renderParameters);
		}
	}
}
