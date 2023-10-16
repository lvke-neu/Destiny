#pragma once

struct ID3D11Device;
struct ID3D11ShaderResourceView;
namespace Destiny
{
	class Texture
	{
	public:
		Texture(ID3D11Device* device, const wchar_t* fileName);
		~Texture();
	public:
		ID3D11ShaderResourceView* m_pShaderResourceView;
	};
}