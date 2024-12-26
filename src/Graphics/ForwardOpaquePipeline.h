#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny 
{
	class ForwardOpaquePipeline : public GraphicsPipeline
	{
	public:
		ForwardOpaquePipeline();
		virtual ~ForwardOpaquePipeline();
	};
}