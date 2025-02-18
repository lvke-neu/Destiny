#include "PostProcessPipeline.h"
#include "BindRenderTargets.h"

namespace Destiny
{
	PostProcessPipeline::PostProcessPipeline() :
		m_bindRenderTargets(std::make_shared<BindRenderTargets>())
	{

	}

	PostProcessPipeline::~PostProcessPipeline()
	{

	}

	void PostProcessPipeline::syncState()
	{
		GraphicsPipeline::syncState();
		addGraphicsCommand(m_bindRenderTargets);
	}
}