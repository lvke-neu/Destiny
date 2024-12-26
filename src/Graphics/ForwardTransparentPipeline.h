#pragma once
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Destiny 
{
	class ForwardTransparentPipeline : public GraphicsPipeline
	{
	public:
		ForwardTransparentPipeline();
		virtual ~ForwardTransparentPipeline();
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) override;
	private:
		void sort();
	};
}