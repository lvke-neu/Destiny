#pragma once

struct ID3D11Device;
struct ID3D11PixelShader;
namespace Destiny
{
	class PixelShader
	{
	public:
		PixelShader(ID3D11Device* device, const wchar_t* fileName);
		~PixelShader();
	public:
		ID3D11PixelShader* m_pPixelShader;
	};
}