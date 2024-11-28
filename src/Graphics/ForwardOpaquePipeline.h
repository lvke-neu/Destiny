#pragma once
#include "GraphicsPipeline.h"

namespace Destiny 
{
	class ForwardOpaquePipeline : public GraphicsPipeline
	{
	public:
		ForwardOpaquePipeline();
		virtual ~ForwardOpaquePipeline();
	public:
		void syncState();
	};
}