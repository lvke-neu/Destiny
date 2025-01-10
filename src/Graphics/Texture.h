#pragma once
#include "Engine/Asset.h"
#include "Engine/Object.h"
#include <string>
#include <unordered_map>

struct ID3D11Texture2D;
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

	class Blob;
	class TextureCreationParam : public Object
	{
	public:
		enum CreateTextureType
		{
			None,
			Create2D,
			CreateTextureTypeCount
		};
	public:
		CreateTextureType m_type	= CreateTextureType::None;
		int format					= -1;
		unsigned int width			= 0;
		unsigned int height			= 0;

		std::shared_ptr<Blob> data	= nullptr;
		unsigned int pitch			= 0;
		unsigned int slicePitch		= 0;
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
		static std::shared_ptr<Texture> Create2D(int format, unsigned int width, unsigned int height, std::shared_ptr<Blob> data, unsigned int pitch, unsigned int slicePitch);
		static std::unordered_map<std::string, std::shared_ptr<Texture>> s_cache;
	public:
		void bind(std::shared_ptr<TextureDesc> desc);
		void unBind(std::shared_ptr<TextureDesc> desc);
		ID3D11ShaderResourceView* getShaderResourceView();
	private:
		ID3D11Texture2D* m_resource;
		ID3D11ShaderResourceView* m_shaderResourceView;
	};

	inline ID3D11ShaderResourceView* Texture::getShaderResourceView()
	{
		return m_shaderResourceView;
	}

}