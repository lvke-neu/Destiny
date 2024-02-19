#pragma once
#include "Visual3DComponent.h"
#include "Graphics/ConstantBuffer.h"
#include <memory>
#include <DirectXMath.h>

namespace Destiny
{
	class PBRSphereComponent : public Visual3DComponent
	{
		RTTR_ENABLE(Visual3DComponent);
	public:
		PBRSphereComponent();
	public:
		GET_CLASS_NAME(PBRSphereComponent);
		GET(Color, albedo);
		GET(float, metallic);
		GET(float, roughness);
		GET(float, ao);
		void set_albedo(Color albedo);
		void set_metallic(float metallic);
		void set_roughness(float roughness);
		void set_ao(float ao);
	private:
		void updateConstantbuffer();
	private:
		struct cbPBRMaterial
		{
			XMFLOAT4 albedo;
			float metallic;
			float roughness;
			float ao;
			float pad;
		};
		std::shared_ptr<ConstantBuffer<cbPBRMaterial>> m_pbrMaterial;
		Color m_albedo;
		float m_metallic;
		float m_roughness;
		float m_ao;
	};
}