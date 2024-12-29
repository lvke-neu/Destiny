#include "GraphicsCommandList.h"
#include "GraphicsCommand.h"

namespace Destiny
{
	bool GraphicsCommandList::addGraphicsCommand(std::shared_ptr<Pre_Post_Process_Command> pre_post_process_command, std::shared_ptr<GraphicsCommand> graphicsCommand)
	{
		if (graphicsCommand)
		{
			return m_graphicsCommandList[pre_post_process_command].insert(graphicsCommand).second;
		}

		return false;
	}

	void GraphicsCommandList::execute(ID3D11DeviceContext* deviceContext)
	{
		for (const auto& graphicsCommandMap : m_graphicsCommandList)
		{
			auto pre_post_process_command = graphicsCommandMap.first;
			
			if (pre_post_process_command && pre_post_process_command->Pre_Process_Command)
			{
				pre_post_process_command->Pre_Process_Command->execute(deviceContext);
			}

			for (const auto& graphicsCommand : graphicsCommandMap.second)
			{
				if (graphicsCommand)
				{
					graphicsCommand->execute(deviceContext);
				}
			}

			if (pre_post_process_command && pre_post_process_command->Post_Process_Command)
			{
				pre_post_process_command->Post_Process_Command->execute(deviceContext);
			}
		}
	}

	void GraphicsCommandList::clearGraphicsCommand()
	{
		m_graphicsCommandList.clear();
	}
}