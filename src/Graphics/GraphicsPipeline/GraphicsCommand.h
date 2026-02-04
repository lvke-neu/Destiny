#pragma once
#include <memory>
#include "../RHI/GraphicsContext.h"

// struct ID3D11DeviceContext;
namespace Destiny
{
	class GraphicsCommand
	{
	public:
		virtual ~GraphicsCommand() = default;
	public:
		virtual void execute(std::shared_ptr<GraphicsContext> deviceContext) = 0;
	};
}