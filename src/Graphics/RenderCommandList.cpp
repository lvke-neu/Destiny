#include "RenderCommandList.h"
#include "RenderCommand.h"

namespace Destiny
{
	void RenderCommandList::addRenderCommand(std::shared_ptr<RenderCommand> renderCommand)
	{
		if (renderCommand)
		{
			m_renderCommandList.push_back(renderCommand);
		}
	}
	void RenderCommandList::execute(ID3D11DeviceContext* deviceContext)
	{
		for (const auto& renderCommand : m_renderCommandList)
		{
			if (renderCommand)
			{
				renderCommand->execute(deviceContext);
			}
		}
	}
}