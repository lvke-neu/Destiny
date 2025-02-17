#include "PostProcessPipeline.h"
#include "DrawParameters.h"
#include "DrawCommand.h"
#include "Mesh.h"
#include "MeshProvider.h"
#include "Renderer.h"
#include "RenderStates.h"
#include "BindRenderTargets.h"

namespace Destiny
{
	PostProcessPipeline::PostProcessPipeline()
	{
		m_bindRenderTargets = std::make_shared<BindRenderTargets>();

		auto fullScreenTriangle = MeshProvider::Create_FullScreenTriangle();
		fullScreenTriangle->load(0);

		auto renderer = std::make_shared<Renderer>("builtin://renderer/full_screen_triangle.hlsl");
		renderer->load(0);

		auto renderStates = std::make_shared<RenderStates>();
		renderStates->load(0);

		auto drawParameters = std::make_shared<DrawParameters>();
		fullScreenTriangle->fillDrawParameters(drawParameters, nullptr);
		renderer->fillDrawParameters(drawParameters);
		renderStates->fillDrawParameters(drawParameters);

		auto drawCommand = std::make_shared<DrawCommand>();
		drawCommand->addDrawParameter(drawParameters);

		addGraphicsCommand(m_bindRenderTargets);
		//addGraphicsCommand(drawCommand);
	}

	PostProcessPipeline::~PostProcessPipeline()
	{

	}
}