#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny
{
	class BindRenderTargets;
	class GuiPipeline : public GraphicsPipeline
	{
	public:
		GuiPipeline();
		virtual ~GuiPipeline();
	public:
		virtual void syncState() override;
	private:
		std::shared_ptr<BindRenderTargets> m_bindRenderTargets;
	};
}