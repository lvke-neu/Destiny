#include "GraphicsCommandList.h"
#include "GraphicsCommand.h"

namespace Destiny
{
	bool GraphicsCommandList::addGraphicsCommand(std::shared_ptr<GraphicsCommand> graphicsCommand)
	{
		if (graphicsCommand)
		{
			return m_graphicsCommandList.insert(graphicsCommand).second;
		}

		return false;
	}

	void GraphicsCommandList::execute(ID3D11DeviceContext* deviceContext)
	{
		for (const auto& graphicsCommand : m_graphicsCommandList)
		{
			if (graphicsCommand)
			{
				graphicsCommand->execute(deviceContext);
			}
		}
	}

	void GraphicsCommandList::clearGraphicsCommand()
	{
		m_graphicsCommandList.clear();
	}
}