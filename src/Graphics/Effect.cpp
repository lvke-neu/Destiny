#include "Effect.h"
#include "Renderer.h"
#include "RenderStates.h"
#include "RenderParameters.h"

namespace Destiny
{
	Effect::Effect(const char* path) :
		m_renderer(std::make_shared<Renderer>(path)),
		m_renderStates(std::make_shared<RenderStates>())
	{

	}

	Effect::~Effect()
	{

	}

	void Effect::doLoad()
	{
		if (!m_renderer || !m_renderStates)
		{
			loadFailed__();
			return;
		}

		if (m_renderer->isLoadingPending())
		{
			m_renderer->load(0);
		}

		if (m_renderStates->isLoadingPending())
		{
			m_renderStates->load(0);
		}

		if (!m_renderer->isLoadingSucceed() || !m_renderStates->isLoadingSucceed())
		{
			loadFailed__();
			return;
		}

		loadSucceeded__();
	}

	void Effect::fillRenderParameters(std::shared_ptr<RenderParameters> renderParameters)
	{
		if (!renderParameters || !m_renderer || !m_renderStates)
		{
			return;
		}

		renderParameters->vertexShader		= m_renderer->m_vertexShader;
		renderParameters->pixelShader		= m_renderer->m_pixelShader;
		renderParameters->rasterizerState	= m_renderStates->m_rasterizerState;
		renderParameters->depthStencilState = m_renderStates->m_depthStencilState;
		renderParameters->blendState		= m_renderStates->m_blendState;
	}

	std::shared_ptr<Blob> Effect::getInputSignatureBlob()
	{
		return m_renderer ? m_renderer->getInputSignatureBlob() : nullptr;
	}
}