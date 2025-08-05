#pragma once
#include <unordered_set>
#include <memory>

struct ID3D11DeviceContext;
namespace Destiny
{
	class GraphicsCommand;
	class GraphicsCommandList
	{
	public:
		virtual ~GraphicsCommandList() = default;
	public:
		bool			addGraphicsCommand(std::shared_ptr<GraphicsCommand> graphicsCommand);
		virtual void	execute(ID3D11DeviceContext* deviceContext);
		void			clearGraphicsCommand();
	protected:
		std::vector<std::shared_ptr<GraphicsCommand>> m_graphicsCommandList;
	};
}