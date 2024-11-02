#include "DrawCommand.h"
#include "Mesh.h"
#include "Renderer.h"
#include "RenderStates.h"

namespace Destiny
{
	DrawCommand::DrawCommand() : 
		m_mesh(nullptr)
	{
	}

	void DrawCommand::setMesh(std::shared_ptr<Mesh> mesh)
	{
		m_mesh = mesh;
	}

	void DrawCommand::setRenderer(std::shared_ptr<Renderer> renderer)
	{
		m_renderer = renderer;
	}

	void DrawCommand::setRenderStates(std::shared_ptr<RenderStates> renderStates)
	{
		m_renderStates = renderStates;
	}

	void DrawCommand::execute(ID3D11DeviceContext* deviceContext)
	{
		if (m_mesh)
		{
			
		}
	}
}