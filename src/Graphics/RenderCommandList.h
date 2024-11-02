#pragma once
#include <vector>
#include <memory>

struct ID3D11DeviceContext;
namespace Destiny
{
	class RenderCommand;
	class RenderCommandList
	{
	public:
		void addRenderCommand(std::shared_ptr<RenderCommand> renderCommand);
		void execute(ID3D11DeviceContext* deviceContext);
	private:
		std::vector<std::shared_ptr<RenderCommand>> m_renderCommandList;
	};
}