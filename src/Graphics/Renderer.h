#pragma once
#include "Engine/Asset.h"

struct ID3D11PixelShader;
namespace Destiny
{
	class Renderer : public Asset
	{
	public:
		Renderer();
		virtual ~Renderer();
	public:
		virtual void doLoad() override;
	private:
		ID3D11PixelShader* m_psShader;
	};
}