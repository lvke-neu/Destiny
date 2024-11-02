#include "GraphicsPipeline.h"
#include "RenderCommandList.h"

namespace Destiny
{
	GraphicsPipeline::GraphicsPipeline() :
		m_renderCommandList(std::make_shared<RenderCommandList>())
	{
	}

	void GraphicsPipeline::execute(ID3D11DeviceContext* deviceContext)
	{
		if (!deviceContext)
		{
			return;
		}
		m_renderCommandList->execute(deviceContext);
	}

	void GraphicsPipeline::addRenderCommand(std::shared_ptr<RenderCommand> renderCommand)
	{
		m_renderCommandList->addRenderCommand(renderCommand);
	}

	void GraphicsPipeline::clearRenderCommand()
	{
		m_renderCommandList->clear();
	}
}
