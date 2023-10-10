#pragma once

struct ID3D11CommandList;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
namespace Destiny
{
	class ImagePass
	{
	public:
		ImagePass();
		~ImagePass();
		void draw();
	public:
		ID3D11CommandList* m_pD3D11CommandList;
		ID3D11InputLayout* m_pD3D11InputLayout;
		ID3D11Buffer* m_pD3D11VertexBuffer;
		ID3D11VertexShader* m_pD3D11VertexShader;
		ID3D11PixelShader* m_pD3D11PixelShader;
	};
}