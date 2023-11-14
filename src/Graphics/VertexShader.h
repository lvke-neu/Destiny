#pragma once
#include "Engine/Asset.h"

struct ID3D11VertexShader;
namespace Destiny
{
	class VertexShader : public Asset
	{
	public:
		VertexShader();
		virtual ~VertexShader();
	public:
		virtual void doLoad() override;
		ID3D11VertexShader* getVertexShader();
	private:
		ID3D11VertexShader* m_vertexShader;
	};

	inline ID3D11VertexShader* VertexShader::getVertexShader()
	{
		return m_vertexShader;
	}
}