#pragma once
#include "GraphicsCommandList.h"

namespace Destiny
{
	class GraphicsPipeline : public GraphicsCommandList
	{
	public:
		virtual ~GraphicsPipeline() = default;
	public:
		virtual void syncState() = 0;
	};
}