#include "GuiPipeline.h"
#include "BindRenderTargets.h"

namespace Destiny
{
	GuiPipeline::GuiPipeline() :
		m_bindRenderTargets(std::make_shared<BindRenderTargets>())
	{

	}

	GuiPipeline::~GuiPipeline()
	{

	}

	void GuiPipeline::syncState()
	{
		GraphicsPipeline::syncState();
		addGraphicsCommand(m_bindRenderTargets);
	}
}