#include "ForwardOpaquePipeline.h"
#include "BindRenderTargets.h"

namespace Destiny
{
	ForwardOpaquePipeline::ForwardOpaquePipeline() :
		m_bindRenderTargets(std::make_shared<BindRenderTargets>())
	{
		
	}

	ForwardOpaquePipeline::~ForwardOpaquePipeline()
	{
		
	}

	void ForwardOpaquePipeline::syncState()
	{
		GraphicsPipeline::syncState();
		addGraphicsCommand(m_bindRenderTargets);
	}
}