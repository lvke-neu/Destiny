#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny 
{
	class TransparentPipeline : public GraphicsPipeline
	{
	public:
		TransparentPipeline(std::shared_ptr<RenderSystem> renderSystem);
		virtual ~TransparentPipeline();
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	private:
		void sort();
	};
}