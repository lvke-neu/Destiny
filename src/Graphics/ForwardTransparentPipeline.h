#pragma once
#include "GraphicsPipeline.h"

namespace Destiny 
{
	class RenderTargetView;
	class BindRenderTargets;
	class ForwardTransparentPipeline : public GraphicsPipeline
	{
	public:
		ForwardTransparentPipeline();
		virtual ~ForwardTransparentPipeline();
	public:
		void syncState();
	};
}