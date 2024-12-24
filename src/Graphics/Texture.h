#pragma once
#include "Engine/Asset.h"
#include <string>
#include <unordered_map>

struct ID3D11Resource;
struct ID3D11ShaderResourceView;
namespace Destiny
{
	enum class TextureBindFlag
	{
		BindVS,
		BindPS,
		BindGS
	};

	struct TextureDesc
	{
		TextureDesc() :
			startSlot(-1)
		{
			textureBindFlag =
			{
				{TextureBindFlag::BindVS, false },
				{TextureBindFlag::BindPS, false },
				{TextureBindFlag::BindGS, false }
			};
		}
		std::unordered_map<TextureBindFlag, bool> textureBindFlag;
		unsigned int startSlot;
	};

	class TextureLoader;
	class Texture : public Asset
	{
		friend class TextureLoader;
	public:
		Texture();
		virtual ~Texture();
	public:
		static std::shared_ptr<TextureLoader> s_textureLoader;
		static std::shared_ptr<Texture> Create(const char* path);
	public:
		void bind(std::shared_ptr<TextureDesc> desc);
		ID3D11ShaderResourceView* getShaderResourceView();
	private:
		ID3D11Resource* m_resource;
		ID3D11ShaderResourceView* m_shaderResourceView;
	};

	inline ID3D11ShaderResourceView* Texture::getShaderResourceView()
	{
		return m_shaderResourceView;
	}

}