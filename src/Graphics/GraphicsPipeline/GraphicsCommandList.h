#pragma once
#include <unordered_map>
#include <unordered_set>
#include <memory>

struct ID3D11DeviceContext;
namespace Destiny
{
	class GraphicsCommand;
	struct Pre_Post_Process_Command
	{
		std::shared_ptr<GraphicsCommand> Pre_Process_Command = nullptr;
		std::shared_ptr<GraphicsCommand> Post_Process_Command = nullptr;
	};

	class GraphicsCommandList
	{
	public:
		virtual ~GraphicsCommandList() = default;
	public:
		bool			addGraphicsCommand(std::shared_ptr<Pre_Post_Process_Command> pre_post_process_command, std::shared_ptr<GraphicsCommand> graphicsCommand);
		virtual void	execute(ID3D11DeviceContext* deviceContext);
		void			clearGraphicsCommand();
	protected:
		std::unordered_map<std::shared_ptr<Pre_Post_Process_Command>, std::unordered_set<std::shared_ptr<GraphicsCommand>>> m_graphicsCommandList;
	};
}