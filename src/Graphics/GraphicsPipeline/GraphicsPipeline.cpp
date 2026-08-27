#include "GraphicsPipeline.h"
#include "../RenderSystem.h"
#include "../BindRenderTargets.h"

namespace Destiny
{
	GraphicsPipeline::GraphicsPipeline(std::shared_ptr<RenderSystem> renderSystem) :
		m_renderSystem(renderSystem)
	{

	}

	void GraphicsPipeline::syncState()
	{
		clearGraphicsCommand();
	}

	void GraphicsPipeline::execute(ID3D11DeviceContext* deviceContext)
	{
		if (m_renderSystem && m_renderSystem->m_bindRenderTargets)
		{
			m_renderSystem->m_bindRenderTargets->execute(deviceContext);
		}
		
		GraphicsCommandList::execute(deviceContext);
	}
}
