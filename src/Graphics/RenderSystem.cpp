#include "RenderSystem.h"
#include "ForwardOpaquePipeline.h"

namespace Destiny
{
	void RenderSystem::createPipeline()
	{
		m_forwardOpaquePipeline = std::make_shared<ForwardOpaquePipeline>();
	}

	void RenderSystem::render()
	{
		m_forwardOpaquePipeline->execute(getImmediateContext());
	}

	void RenderSystem::syncState()
	{
		m_forwardOpaquePipeline->syncState();
	}
}