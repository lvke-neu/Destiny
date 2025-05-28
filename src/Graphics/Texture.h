#pragma once
#include "Engine/Asset.h"
#include "Engine/Object.h"
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

	class BlobHolder;
	class HdrCreationParma : public Object
	{
	public:
		enum CreateTextureType
		{
			None,
			Hdr,
			Irradiance,
			Prefilter,
			CreateTextureTypeCount
		};
	public:
		static std::string mapTypeToString(CreateTextureType type)
		{
			switch (type)
			{
			case Destiny::HdrCreationParma::Hdr:
				return "Hdr";
			case Destiny::HdrCreationParma::Irradiance:
				return "Irradiance";
			case Destiny::HdrCreationParma::Prefilter:
				return "Prefilter";
			case Destiny::HdrCreationParma::None:
			case Destiny::HdrCreationParma::CreateTextureTypeCount:
			default:
				return "";
			}
		}
	public:
		CreateTextureType m_type = CreateTextureType::None;
		std::shared_ptr<BlobHolder> m_blobHolder = nullptr;
	};

	class TextureLoader;
	class Texture : public Asset
	{
		friend class TextureLoader;
	public:
		Texture();
		Texture(ID3D11Resource* resource, ID3D11ShaderResourceView* shaderResourceView);
		virtual ~Texture();
	public:
		static std::shared_ptr<TextureLoader> s_textureLoader;
		static std::shared_ptr<Texture> Create(const char* path);
		static std::shared_ptr<Texture> Create2D(int format, unsigned int width, unsigned int height, std::shared_ptr<Blob> data, unsigned int pitch, unsigned int slicePitch);
		static std::shared_ptr<Texture> CreateHdr(const char* path, HdrCreationParma::CreateTextureType type);
		static std::unordered_map<std::string, std::shared_ptr<Texture>> s_cache;
		static void Hdr_To_Cube_Irradiance_Prefilter_DDS();
	public:
		std::string getPath();
	public:
		void bind(std::shared_ptr<TextureDesc> desc);
		void unBind(std::shared_ptr<TextureDesc> desc);
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