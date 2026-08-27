#pragma once

struct ID3D11DeviceContext;
namespace Destiny
{
	class GraphicsCommand
	{
	public:
		virtual ~GraphicsCommand() = default;
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) = 0;
	};
}