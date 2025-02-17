#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny
{
	class BindRenderTargets;
	class PostProcessPipeline : public GraphicsPipeline
	{
	public:
		PostProcessPipeline();
		virtual ~PostProcessPipeline();
	public:
		std::shared_ptr<BindRenderTargets> m_bindRenderTargets;
	};
}