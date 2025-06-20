#pragma once
#include "Graphics/Material.h"
#include <string>

namespace Destiny
{
	class SamplerState;
	class Texture;
	class PbrMaterial : public Material
	{
		RTTR_ENABLE(Material);
	public:
		PbrMaterial();
		virtual ~PbrMaterial() = default;
	public:
		static std::shared_ptr<PbrMaterial> Create_Default();
		static std::shared_ptr<PbrMaterial> Create_Gold();
		static std::shared_ptr<PbrMaterial> Create_Grass();
		static std::shared_ptr<PbrMaterial> Create_Peel();
		static std::shared_ptr<PbrMaterial> Create_Plastic();
		static std::shared_ptr<PbrMaterial> Create_Rusted_Iron();
		static std::shared_ptr<PbrMaterial> Create_Wall();
		static std::shared_ptr<PbrMaterial> Create_Water();
	public: 
		std::string get_albedo();
		void set_albedo(std::string albedo);
		void setAlbedo(std::shared_ptr<Texture> texture);
		std::string get_normal();
		void set_normal(std::string normal);
		void setNormal(std::shared_ptr<Texture> texture);
		std::string get_metallic();
		void set_metallic(std::string metallic);
		void setMetallic(std::shared_ptr<Texture> texture);
		std::string get_roughness();
		void set_roughness(std::string roughness);
		void setRoughness(std::shared_ptr<Texture> texture);
		std::string get_ao();
		void set_ao(std::string ao);
		void setAo(std::shared_ptr<Texture> texture);
	private:
		virtual void update() override;
	private:
		std::shared_ptr<Texture>		m_albedo;
		std::shared_ptr<Texture>		m_normal;
		std::shared_ptr<Texture>		m_metallic;
		std::shared_ptr<Texture>		m_roughness;
		std::shared_ptr<Texture>		m_ao;

		std::shared_ptr<SamplerState>	m_sampler;
	};
}