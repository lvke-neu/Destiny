#include "PostProcessPipeline.h"
#include "BindRenderTargets.h"
#include "FullScreenTriangleCommand.h"

namespace Destiny
{
	PostProcessPipeline::PostProcessPipeline() :
		m_bindRenderTargets(std::make_shared<BindRenderTargets>()),
		m_fullScreenTriangleCommand(std::make_shared<FullScreenTriangleCommand>())
	{

	}

	PostProcessPipeline::~PostProcessPipeline()
	{

	}

	void PostProcessPipeline::syncState()
	{
		GraphicsPipeline::syncState();
		addGraphicsCommand(m_bindRenderTargets);
		addGraphicsCommand(m_fullScreenTriangleCommand);
	}
}