#include "FullScreenTriangleCommand.h"
#include "Mesh.h"
#include "MeshProvider.h"
#include "Renderer.h"
#include "RenderStates.h"
#include "DrawParameters.h"

namespace Destiny
{
	FullScreenTriangleCommand::FullScreenTriangleCommand()
	{
		auto mesh = MeshProvider::Create_FullScreenTriangle();
		mesh->load(0);

		auto renderer = std::make_shared<Renderer>("builtin://renderer/full_screen_triangle.hlsl");
		renderer->load(0);

		auto renderstates = std::make_shared<RenderStates>();
		renderstates->load(0);

		auto drawParameters = std::make_shared<DrawParameters>();
		mesh->fillDrawParameters(drawParameters, nullptr);
		renderer->fillDrawParameters(drawParameters);
		renderstates->fillDrawParameters(drawParameters);

		addDrawParameter(drawParameters);
	}
}