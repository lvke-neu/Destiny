#include "GraphicsCommandList.h"
#include "GraphicsCommand.h"

namespace Destiny
{
	bool GraphicsCommandList::addGraphicsCommand(std::shared_ptr<GraphicsCommand> graphicsCommand)
	{
		if (graphicsCommand)
		{
			auto find = std::find(m_graphicsCommandList.begin(), m_graphicsCommandList.end(), graphicsCommand);
			if (find != m_graphicsCommandList.end())
			{
				return false;
			}
			
			m_graphicsCommandList.push_back(graphicsCommand);
			return true;
		}

		return false;
	}

	void GraphicsCommandList::removeGraphicsCommand(std::shared_ptr<GraphicsCommand> graphicsCommand)
	{
		auto iter = std::find(m_graphicsCommandList.begin(), m_graphicsCommandList.end(), graphicsCommand);
		if (iter != m_graphicsCommandList.end())
		{
			m_graphicsCommandList.erase(iter);
		}
	}

	void GraphicsCommandList::execute(std::shared_ptr<GraphicsContext> deviceContext)
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