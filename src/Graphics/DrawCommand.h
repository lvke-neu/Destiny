#pragma once
#include "RenderCommand.h"
#include <memory>

namespace Destiny
{
	class Mesh;
	class Renderer;
	class RenderStates;
	class DrawCommand : public RenderCommand
	{
	public:
		DrawCommand();
		virtual ~DrawCommand() = default;
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	public:
		void setMesh(std::shared_ptr<Mesh> mesh);
		void setRenderer(std::shared_ptr<Renderer> renderer);
		void setRenderStates(std::shared_ptr<RenderStates> renderStates);
	private:
		std::shared_ptr<Mesh> m_mesh;
		std::shared_ptr<Renderer> m_renderer;
		std::shared_ptr<RenderStates> m_renderStates;
	};
}