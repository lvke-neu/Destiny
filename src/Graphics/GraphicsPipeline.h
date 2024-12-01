#pragma once
#include "RenderCommandList.h"

namespace Destiny
{
	class GraphicsPipeline : public RenderCommandList
	{
	public:
		virtual ~GraphicsPipeline() = default;
	public:
		virtual void syncState() = 0;
	};
}