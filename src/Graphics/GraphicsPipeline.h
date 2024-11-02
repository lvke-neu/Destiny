#pragma once
#include <memory>

struct ID3D11DeviceContext;
namespace Destiny
{
	class RenderCommand;
	class RenderCommandList;
	class GraphicsPipeline
	{
	public:
		GraphicsPipeline();
		virtual ~GraphicsPipeline() = default;
	public:
		void execute(ID3D11DeviceContext* deviceContext);
		void addRenderCommand(std::shared_ptr<RenderCommand> renderCommand);
	private:
		std::shared_ptr<RenderCommandList> m_renderCommandList;
	};
}