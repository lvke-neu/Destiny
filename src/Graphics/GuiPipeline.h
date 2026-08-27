#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny
{
	class GuiPipeline : public GraphicsPipeline
	{
	public:
		GuiPipeline(std::shared_ptr<RenderSystem> renderSystem);
		virtual ~GuiPipeline();
	};
}