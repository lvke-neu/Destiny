#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny 
{
	class ForwardOpaquePipeline : public GraphicsPipeline
	{
	public:
		ForwardOpaquePipeline(std::shared_ptr<RenderSystem> renderSystem);
		virtual ~ForwardOpaquePipeline();
	};
}