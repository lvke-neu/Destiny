#pragma once
#include <unordered_set>
#include <memory>

struct ID3D11DeviceContext;
namespace Destiny
{
	class RenderCommand;
	class RenderCommandList
	{
	public:
		virtual ~RenderCommandList() = default;
	public:
		bool addRenderCommand(std::shared_ptr<RenderCommand> renderCommand);
		virtual void execute(ID3D11DeviceContext* deviceContext);
		void clearRenderCommand();
	protected:
		std::unordered_set<std::shared_ptr<RenderCommand>> m_renderCommandList;
	};
}