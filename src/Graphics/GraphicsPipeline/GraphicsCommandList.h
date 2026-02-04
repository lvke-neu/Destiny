#pragma once
#include <unordered_set>
#include <memory>
#include <vector>
#include "../RHI/GraphicsContext.h"

// struct ID3D11DeviceContext;
namespace Destiny
{
	class GraphicsCommand;
	class GraphicsCommandList
	{
	public:
		virtual ~GraphicsCommandList() = default;
	public:
		bool			addGraphicsCommand(std::shared_ptr<GraphicsCommand> graphicsCommand);
		void			removeGraphicsCommand(std::shared_ptr<GraphicsCommand> graphicsCommand);
		virtual void	execute(std::shared_ptr<GraphicsContext> deviceContext);
		void			clearGraphicsCommand();
	protected:
		std::vector<std::shared_ptr<GraphicsCommand>> m_graphicsCommandList;
	};
}