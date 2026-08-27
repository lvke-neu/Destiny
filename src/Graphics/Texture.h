#pragma once
#include "Engine/Asset.h"
#include "Engine/Object.h"
#include <string>
#include <unordered_map>


struct ID3D11Resource;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;
namespace Destiny
{
	enum class TextureBindFlag
	{
		BindVS,
		BindPS,
		BindGS,
		BindHS,
		BindDS,
		BindCS
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
				{TextureBindFlag::BindGS, false },
				{TextureBindFlag::BindHS, false },
				{TextureBindFlag::BindDS, false },
				{TextureBindFlag::BindCS, false }
			};
		}
		std::unordered_map<TextureBindFlag, bool> textureBindFlag;
		unsigned int startSlot;
	};

	class Blob;
	class TextureCreationParam : public Object
	{
	public:
		enum class CreateTextureType
		{
			None,
			CreateBuffer,
			Create2DSRV,
			Create2DUAV,
			CreateStructured,
			CreateTyped,
			CreateRaw,

			CreateTextureTypeCount
		};

		enum class CreateStructuredType
		{
			None = 0,
			Append = 0x2,
			Counter = 0x4
		};
	public:
		CreateTextureType m_type	= CreateTextureType::None;
		int format					= -1;
		unsigned int width			= 0;
		unsigned int height			= 0;

		std::shared_ptr<Blob> data	= nullptr;
		union 
		{
			//for Create2DSRV
			unsigned int pitch = 0;
			//for CreateStructured
			unsigned int structuredBufferByteStride;
			//for CreateTyped
			unsigned int typedBufferByteStride;
			//for CreateRaw
			unsigned int rawBufferWidth;
		};
		union
		{
			//for buffer
			unsigned int bufferByteWidth = 0;
			//for Create2DSRV
			unsigned int slicePitch;
			//for CreateStructured
			unsigned int structuredBufferByteWidth;
			//for CreateTyped
			unsigned int typedBufferByteWidth;
		};

		union 
		{
			CreateStructuredType  createStructuredType = CreateStructuredType::None;
		};
		
		bool isProc = false;

		std::string procPath = "";
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
		friend class ComputerCommand;
	public:
		Texture();
		Texture(ID3D11Resource* resource, ID3D11ShaderResourceView* shaderResourceView);
		virtual ~Texture();
	public:
		static std::shared_ptr<TextureLoader> s_textureLoader;
		static std::shared_ptr<Texture> Create(const char* path);
		static std::shared_ptr<Texture> Create(std::shared_ptr<Blob> blob, const char* type);
		static std::shared_ptr<Texture> CreateBuffer(unsigned int bufferByteWidth);
		static std::shared_ptr<Texture> Create2DSRV(int format, unsigned int width, unsigned int height, std::shared_ptr<Blob> data, unsigned int pitch, unsigned int slicePitch, bool isProc = false, const std::string& procPath = "");
		static std::shared_ptr<Texture> Create2DUAV(int format, unsigned int width, unsigned int height);
		static std::shared_ptr<Texture> CreateStructured(unsigned int structuredBufferByteStride, unsigned int structuredBufferByteWidth, std::shared_ptr<Blob> data = nullptr);
		static std::shared_ptr<Texture> CreateStructuredAppend(unsigned int structuredBufferByteStride, unsigned int structuredBufferByteWidth, std::shared_ptr<Blob> data = nullptr);
		static std::shared_ptr<Texture> CreateStructuredCounter(unsigned int structuredBufferByteStride, unsigned int structuredBufferByteWidth, std::shared_ptr<Blob> data = nullptr);
		static std::shared_ptr<Texture> CreateTyped(int format, unsigned int typedBufferByteStride, unsigned int typedBufferByteWidth, std::shared_ptr<Blob> data = nullptr);
		static std::shared_ptr<Texture> CreateRaw(unsigned int rawBufferWidth, std::shared_ptr<Blob> data = nullptr);
		static std::shared_ptr<Texture> CreateHdr(const char* path, HdrCreationParma::CreateTextureType type);

		static std::unordered_map<std::string, std::shared_ptr<Texture>> s_cache;
		static void Hdr_To_Cube_Irradiance_Prefilter_DDS();
	public:
		std::string getPath();
	public:
		void bind(std::shared_ptr<TextureDesc> desc);
		void unBind(std::shared_ptr<TextureDesc> desc);
		ID3D11Resource* getResource();
		ID3D11ShaderResourceView* getShaderResourceView();
		ID3D11UnorderedAccessView** getUnorderedAccessView();
		void updateBuffer(std::shared_ptr<Blob> data);
		bool getBuffer(std::shared_ptr<Blob>& data);
	private:
		ID3D11Resource* m_resource;
		ID3D11ShaderResourceView* m_shaderResourceView;
		ID3D11UnorderedAccessView* m_unorderedAccessView;
	};

	inline ID3D11Resource* Texture::getResource()
	{
		return m_resource;
	}

	inline ID3D11ShaderResourceView* Texture::getShaderResourceView()
	{
		return m_shaderResourceView;
	}

	inline ID3D11UnorderedAccessView** Texture::getUnorderedAccessView()
	{
		return &m_unorderedAccessView;
	}
}