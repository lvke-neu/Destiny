#pragma once

struct ID3D11Device;
struct ID3D11VertexShader;
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
namespace Destiny
{
	class VertexShader
	{
	public:
		VertexShader(ID3D11Device* device, const wchar_t* fileName, const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, unsigned int NumElements);
		~VertexShader();
	public:
		ID3D11VertexShader* m_pVertexShader;
		ID3D11InputLayout* m_pInputLayout;
	};
}