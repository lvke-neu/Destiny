#include "RenderCommandList.h"
#include "RenderCommand.h"

namespace Destiny
{
	bool RenderCommandList::addRenderCommand(std::shared_ptr<RenderCommand> renderCommand)
	{
		if (renderCommand)
		{
			return m_renderCommandList.insert(renderCommand).second;
		}

		return false;
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

	void RenderCommandList::clearRenderCommand()
	{
		m_renderCommandList.clear();
	}
}