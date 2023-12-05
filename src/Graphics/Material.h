#pragma once
#include "Engine/Asset.h"
#include "Engine/Reflection.h"
#include "Color.h"

namespace Destiny
{
	class SamplerState;
	class Texture;
	class Material : public Asset
	{
		friend class MaterialLoader;
	public:
		Material();
		virtual ~Material();
	public:
		GET_SET(Color, ambientColor);
		GET_SET(Color, diffuseColor);
		GET_SET(Color, specularColor);
		GET_SET(std::string, ambientTexturePath);
		GET_SET(std::string, diffuseTexturePath);
		GET_SET(std::string, specularTexturePath);
		GET_SET(std::shared_ptr<SamplerState>, ambientSamplerState);
		GET_SET(std::shared_ptr<SamplerState>, diffuseSamplerState);
		GET_SET(std::shared_ptr<SamplerState>, specularSamplerState);

		GET(std::shared_ptr<Texture>, ambientTexture);
		GET(std::shared_ptr<Texture>, diffuseTexture);
		GET(std::shared_ptr<Texture>, specularTexture);
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