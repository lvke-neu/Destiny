#pragma once
#include "Engine/Asset.h"

struct ID3D11GeometryShader;
namespace Destiny
{
	class GeometryShader : public Asset
	{
	public:
		GeometryShader();
		virtual ~GeometryShader();
	public:
		virtual void doLoad() override;
		ID3D11GeometryShader* getGeometryShader();
	private:
		ID3D11GeometryShader* m_geometryShader;
	};

	inline ID3D11GeometryShader* GeometryShader::getGeometryShader()
	{
		return m_geometryShader;
	}
}