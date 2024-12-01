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
		virtual void syncState() override;
	};
}