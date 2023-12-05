#pragma once
#include "Engine/Asset.h"
#include "Engine/Reflection.h"
#include "Color.h"

namespace Destiny
{
	class SamplerState;
	class Texture;
	class Material : public Asset, public Reflection
	{
		friend class MaterialLoader;
		RTTR_ENABLE(Reflection);
	public:
		Material();
		virtual ~Material();
	public:
		GET_CLASS_NAME(Material);
		GET_SET(Color, ambientColor);
		GET_SET(Color, diffuseColor);
		GET_SET(Color, specularColor);
		GET(std::string, ambientTexturePath);
		GET(std::string, diffuseTexturePath);
		GET(std::string, specularTexturePath);
		GET_SET(std::shared_ptr<SamplerState>, ambientSamplerState);
		GET_SET(std::shared_ptr<SamplerState>, diffuseSamplerState);
		GET_SET(std::shared_ptr<SamplerState>, specularSamplerState);

		GET(std::shared_ptr<Texture>, ambientTexture);
		GET(std::shared_ptr<Texture>, diffuseTexture);
		GET(std::shared_ptr<Texture>, specularTexture);

		void set_ambientTexturePath(std::string ambientTexturePath);
		void set_diffuseTexturePath(std::string diffuseTexturePath);
		void set_specularTexturePath(std::string specularTexturePath);
	private:
		Color m_ambientColor;
		Color m_diffuseColor;
		Color m_specularColor;

		std::string m_ambientTexturePath;
		std::string m_diffuseTexturePath;
		std::string m_specularTexturePath;

		std::shared_ptr<Texture> m_ambientTexture;
		std::shared_ptr<Texture> m_diffuseTexture;
		std::shared_ptr<Texture> m_specularTexture;

		std::shared_ptr<SamplerState> m_ambientSamplerState;
		std::shared_ptr<SamplerState> m_diffuseSamplerState;
		std::shared_ptr<SamplerState> m_specularSamplerState;
	};
}