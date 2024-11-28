#pragma once
#include "GraphicsPipeline.h"

namespace Destiny 
{
	class ForwardTransparentPipeline : public GraphicsPipeline
	{
	public:
		ForwardTransparentPipeline();
		virtual ~ForwardTransparentPipeline();
	public:
		void syncState();
	};
}