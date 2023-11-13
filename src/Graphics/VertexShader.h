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
	private:
		ID3D11VertexShader* m_vertexShader;
	};
}