#pragma once
#include "Engine/Asset.h"

struct ID3D11PixelShader;
namespace Destiny
{
	class PixelShader : public Asset
	{
	public:
		PixelShader();
		virtual ~PixelShader();
	public:
		virtual void doLoad() override;
	private:
		ID3D11PixelShader* m_pixelShader;
	};
}