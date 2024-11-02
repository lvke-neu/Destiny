#pragma once

struct ID3D11DeviceContext;
namespace Destiny
{
	class RenderCommand
	{
	public:
		virtual ~RenderCommand() = default;
	public:
		virtual void execute(ID3D11DeviceContext* deviceContext) = 0;
	};
}