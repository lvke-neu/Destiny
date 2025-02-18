#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny 
{
	class BindRenderTargets;
	class TransparentPipeline : public GraphicsPipeline
	{
	public:
		TransparentPipeline();
		virtual ~TransparentPipeline();
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
		virtual void syncState() override;
	private:
		void sort();
	private:
		std::shared_ptr<BindRenderTargets> m_bindRenderTargets;
	};
}