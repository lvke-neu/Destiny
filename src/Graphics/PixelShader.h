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
		ID3D11PixelShader* getPixelShader();
	private:
		ID3D11PixelShader* m_pixelShader;
	};

	inline ID3D11PixelShader* PixelShader::getPixelShader()
	{
		return m_pixelShader;
	}
}