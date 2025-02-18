#pragma once
#include "DrawCommand.h"

namespace Destiny
{
	class FullScreenTriangleCommand : public DrawCommand
	{
	public:
		FullScreenTriangleCommand();
		virtual ~FullScreenTriangleCommand() = default;
	};
}