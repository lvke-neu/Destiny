#include "Texture.h"
#include "TextureLoader.h"
#include "GraphicsSystem.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobHolder.h"
#include "Engine/BlobLoader.h"
#include "Engine/Blob.h"
#include "Engine/Utility.h"
#include <d3d11.h>
#include "HDRTextureLoader.h"
#include <regex>

namespace Destiny
{
	std::shared_ptr<TextureLoader> Texture::s_textureLoader = std::make_shared<TextureLoader>();
	std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::s_cache;
	Texture::Texture() :
		m_resource(nullptr),
		m_shaderResourceView(nullptr),
		m_unorderedAccessView(nullptr),
        m_resourceType(ResourceType::Unknown)
	{

	}

	Texture::Texture(void* resource, void* shaderResourceView) :
		m_resource(resource),
		m_shaderResourceView(shaderResourceView),
		m_unorderedAccessView(nullptr),
        m_resourceType(ResourceType::Texture) // Assume texture for now when created this way
	{
        // No AddRef in Vulkan or void* land. Ownership management needs to be careful.
        // If this constructor takes ownership, we should know.
        // For DepthStencilView, it creates the texture and passes it here. 
        // DepthStencilView manages the lifecycle via SAFE_RELEASE if it were COM, 
        // but here Texture takes ownership? 
        // Looking at DepthStencilView::getTexture(), it creates a new Texture shared_ptr.
        // If Texture destructor destroys m_resource, then DepthStencilView shouldn't destroy it double?
        // Or is Texture just a wrapper?
        
        // In D3D11, AddRef was called. 
        // In Vulkan, we don't have ref counting on VkImage.
        // We need to decide if Texture owns the resource.
        // For now, let's assume shared ownership isn't easily possible without a custom ref counter.
        // But since we use shared_ptr<Texture>, maybe that's enough?
        
        // However, DepthStencilView also holds m_texture.
        // If DepthStencilView dies, it calls SAFE_RELEASE (DestroyTexture).
        // If Texture dies, it calls DestroyTexture.
        // Double free!
        
        // TODO: Implement proper resource reference counting or ownership transfer.
        // For this migration step, we'll assume Texture does NOT own the resource if passed in 
        // (or we need to suppress destruction in Texture if it wasn't created by TextureLoader).
        // But Texture destructor unconditionally destroys.
        
        // TEMPORARY FIX: Mark as Unknown so destructor skips destroy? 
        // Or better, let's just accept we might need to change DepthStencilView to not own it, or use shared_ptr.
        
        // Actually, in the original code:
        // Texture(ID3D11Resource* resource, ...) { AddRef(); }
        // ~Texture() { Release(); }
        // So Texture DID own a reference.
        
        // In Vulkan, we don't have this.
        // We will mark it as Unknown type for now to prevent Texture::~Texture from destroying it,
        // relying on DepthStencilView to destroy it.
        // This effectively makes this Texture a "View" or "Weak Reference".
        m_resourceType = ResourceType::Unknown; 
	}

	Texture::~Texture()
	{
        if (m_resource)
        {
            if (m_resourceType == ResourceType::Texture)
            {
                Engine::GetInstance()->getGraphicsSystem()->getDevice()->DestroyTexture(m_resource);
            }
            else if (m_resourceType == ResourceType::Buffer)
            {
                Engine::GetInstance()->getGraphicsSystem()->getDevice()->DestroyBuffer(m_resource);
            }
            else
            {
                // Fallback for unknown type or D3D11 resources if any (should be avoided)
                // For now, assume it might be a raw pointer that needs special handling or leak if unknown
                // In D3D11 path we would call Release(), but we removed d3d11.h dependency in header
                // We should ensure all creations set m_resourceType
            }
            m_resource = nullptr;
        }

        // Views are currently not managed by GraphicsDevice destructor interface yet
        // In D3D11 they are COM objects. In Vulkan they are likely VkImageView/VkBufferView
        // We need DestroyShaderResourceView etc.
        // For now, just set to nullptr to avoid SAFE_RELEASE crash
		// SAFE_RELEASE(m_shaderResourceView);
		// SAFE_RELEASE(m_unorderedAccessView);
        m_shaderResourceView = nullptr;
        m_unorderedAccessView = nullptr;
	}

	std::shared_ptr<Texture> Texture::Create(const char* path)
	{
		if (std::string(path).find(".hdr") != std::string::npos)
		{
			auto pos = std::string(path).find("?type=irradiance");
			if (pos != std::string::npos)
			{
				return CreateHdr(std::string(path).substr(0, pos).c_str(), HdrCreationParma::Irradiance);
			}

			pos = std::string(path).find("?type=prefilter");
			if (pos != std::string::npos)
			{
				return CreateHdr(std::string(path).substr(0, pos).c_str(), HdrCreationParma::Prefilter);
			}

			return CreateHdr(path, HdrCreationParma::Hdr);
		}
		else if (std::string(path).find("proc://") != std::string::npos)
		{
			struct Pixel
			{
				unsigned char r;
				unsigned char g;
				unsigned char b;
				unsigned char a;
			};

			std::vector<Pixel> res;
			res.resize(1);

			static std::regex s_paramPattern("(\\?|\\&)(r|g|b|a)=([^&]*)");
			const char* str = path + 7;
			const char* it = str;
			std::cmatch result;
			while (std::regex_search(it, result, s_paramPattern))
			{
				if (result[2] == "r")
				{
					res[0].r = atoi(result[3].str().c_str());
				}
				else if (result[2] == "g")
				{
					res[0].g = atoi(result[3].str().c_str());
				}
				else if (result[2] == "b")
				{
					res[0].b = atoi(result[3].str().c_str());
				}
				else if (result[2] == "a")
				{
					res[0].a = atoi(result[3].str().c_str());
				}
				it = result[0].second;
			}

			auto blobData = std::make_shared<Blob>(res.size() * sizeof(Pixel));
			blobData->copyfrom(res.data(), blobData->getLength());

			return Create2DSRV(DXGI_FORMAT_R8G8B8A8_UNORM, 1, 1, blobData, sizeof(Pixel), sizeof(Pixel), true, path);
		}
		auto iter = s_cache.find(path);
		if (iter != s_cache.end())
		{
			return iter->second;
		}

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();

		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		if (blobLoader)
		{
			texture->initialize(s_textureLoader, blobLoader->createBlobHolder(path));
		}

		s_cache.insert({ path, texture });
		return texture;
	}

	std::shared_ptr<Texture> Texture::Create(std::shared_ptr<Blob> blob, const char* type)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();

		std::shared_ptr<BlobHolder> blobHolder = std::make_shared<BlobHolder>(Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader("builtin://"), type);
		blobHolder->loadSucceeded__(blob);

		texture->initialize(s_textureLoader, blobHolder);

		return texture;
	}

	std::shared_ptr<Texture> Texture::CreateBuffer(unsigned int bufferByteWidth)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		std::shared_ptr<TextureCreationParam> creationParam = std::make_shared<TextureCreationParam>();

		creationParam->m_type = TextureCreationParam::CreateTextureType::CreateBuffer;
		creationParam->bufferByteWidth = bufferByteWidth;

		texture->initialize(s_textureLoader, creationParam);

		return texture;
	}

	std::shared_ptr<Texture> Texture::Create2DSRV(int format, unsigned int width, unsigned int height, std::shared_ptr<Blob> data, unsigned int pitch, unsigned int slicePitch, bool isProc, const std::string& procPath)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		std::shared_ptr<TextureCreationParam> creationParam = std::make_shared<TextureCreationParam>();
		
		creationParam->m_type = TextureCreationParam::CreateTextureType::Create2DSRV;
		creationParam->format = format;
		creationParam->width = width;
		creationParam->height = height;
		creationParam->data = data;
		creationParam->pitch = pitch;
		creationParam->slicePitch = slicePitch;
		creationParam->isProc = isProc;
		creationParam->procPath = procPath;
		texture->initialize(s_textureLoader, creationParam);

		return texture;
	}

	std::shared_ptr<Texture> Texture::Create2DUAV(int format, unsigned int width, unsigned int height)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		std::shared_ptr<TextureCreationParam> creationParam = std::make_shared<TextureCreationParam>();

		creationParam->m_type = TextureCreationParam::CreateTextureType::Create2DUAV;
		creationParam->format = format;
		creationParam->width = width;
		creationParam->height = height;
		//creationParam->data = data;
		//creationParam->pitch = pitch;
		//creationParam->slicePitch = slicePitch;
		//creationParam->isProc = isProc;
		//creationParam->procPath = procPath;
		texture->initialize(s_textureLoader, creationParam);

		return texture;
	}

	std::shared_ptr<Texture> Texture::CreateStructured(unsigned int structuredBufferByteStride, unsigned int structuredBufferByteWidth, std::shared_ptr<Blob> data)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		std::shared_ptr<TextureCreationParam> creationParam = std::make_shared<TextureCreationParam>();

		creationParam->m_type = TextureCreationParam::CreateTextureType::CreateStructured;
		creationParam->data = data;
		creationParam->structuredBufferByteStride = structuredBufferByteStride;
		creationParam->structuredBufferByteWidth = structuredBufferByteWidth;
		creationParam->createStructuredType = TextureCreationParam::CreateStructuredType::None;

		texture->initialize(s_textureLoader, creationParam);

		return texture;
	}

	std::shared_ptr<Texture> Texture::CreateStructuredAppend(unsigned int structuredBufferByteStride, unsigned int structuredBufferByteWidth, std::shared_ptr<Blob> data)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		std::shared_ptr<TextureCreationParam> creationParam = std::make_shared<TextureCreationParam>();

		creationParam->m_type = TextureCreationParam::CreateTextureType::CreateStructured;
		creationParam->data = data;
		creationParam->structuredBufferByteStride = structuredBufferByteStride;
		creationParam->structuredBufferByteWidth = structuredBufferByteWidth;
		creationParam->createStructuredType = TextureCreationParam::CreateStructuredType::Append;

		texture->initialize(s_textureLoader, creationParam);

		return texture;
	}

	std::shared_ptr<Texture> Texture::CreateStructuredCounter(unsigned int structuredBufferByteStride, unsigned int structuredBufferByteWidth, std::shared_ptr<Blob> data)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		std::shared_ptr<TextureCreationParam> creationParam = std::make_shared<TextureCreationParam>();

		creationParam->m_type = TextureCreationParam::CreateTextureType::CreateStructured;
		creationParam->data = data;
		creationParam->structuredBufferByteStride = structuredBufferByteStride;
		creationParam->structuredBufferByteWidth = structuredBufferByteWidth;
		creationParam->createStructuredType = TextureCreationParam::CreateStructuredType::Counter;

		texture->initialize(s_textureLoader, creationParam);

		return texture;
	}

	std::shared_ptr<Texture> Texture::CreateTyped(int format, unsigned int typedBufferByteStride, unsigned int typedBufferByteWidth, std::shared_ptr<Blob> data)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		std::shared_ptr<TextureCreationParam> creationParam = std::make_shared<TextureCreationParam>();

		creationParam->m_type = TextureCreationParam::CreateTextureType::CreateTyped;
		creationParam->data = data;
		creationParam->format = format;
		creationParam->typedBufferByteStride = typedBufferByteStride;
		creationParam->typedBufferByteWidth = typedBufferByteWidth;

		texture->initialize(s_textureLoader, creationParam);

		return texture;
	}

	std::shared_ptr<Texture> Texture::CreateRaw(unsigned int rawBufferWidth, std::shared_ptr<Blob> data)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		std::shared_ptr<TextureCreationParam> creationParam = std::make_shared<TextureCreationParam>();

		creationParam->m_type = TextureCreationParam::CreateTextureType::CreateRaw;
		creationParam->data = data;
		creationParam->rawBufferWidth = rawBufferWidth;

		texture->initialize(s_textureLoader, creationParam);

		return texture;
	}

	std::shared_ptr<Texture> Texture::CreateHdr(const char* path, HdrCreationParma::CreateTextureType type)
	{
		auto iter = s_cache.find(path + HdrCreationParma::mapTypeToString(type));
		if (iter != s_cache.end())
		{
			return iter->second;
		}

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();

		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(path);
		if (blobLoader)
		{
			std::shared_ptr<HdrCreationParma> hdrCreationParma = std::make_shared<HdrCreationParma>();
			hdrCreationParma->m_type = type;
			hdrCreationParma->m_blobHolder = blobLoader->createBlobHolder(path);
			texture->initialize(s_textureLoader, hdrCreationParma);
		}

		s_cache.insert({ path + HdrCreationParma::mapTypeToString(type), texture });
		return texture;
	}

	void Texture::Hdr_To_Cube_Irradiance_Prefilter_DDS()
	{
		std::shared_ptr<Texture> tex = nullptr;

		tex = Texture::Create("builtin://texture/skybox/hdr/blue_photo_studio_4k.hdr");
		tex->load();
		//tex = Texture::Create("builtin://texture/skybox/hdr/blue_photo_studio_4k.hdr?type=irradiance");
		//tex->load();
		//tex = Texture::Create("builtin://texture/skybox/hdr/blue_photo_studio_4k.hdr?type=prefilter");
		//tex->load();

		tex = Texture::Create("builtin://texture/skybox/hdr/evening_meadow_4k.hdr");
		tex->load();
		//tex = Texture::Create("builtin://texture/skybox/hdr/evening_meadow_4k.hdr?type=irradiance");
		//tex->load();
		//tex = Texture::Create("builtin://texture/skybox/hdr/evening_meadow_4k.hdr?type=prefilter");
		//tex->load();

		tex = Texture::Create("builtin://texture/skybox/hdr/little_paris_eiffel_tower_4k.hdr");
		tex->load();
		//tex = Texture::Create("builtin://texture/skybox/hdr/little_paris_eiffel_tower_4k.hdr?type=irradiance");
		//tex->load();
		//tex = Texture::Create("builtin://texture/skybox/hdr/little_paris_eiffel_tower_4k.hdr?type=prefilter");
		//tex->load();

		tex = Texture::Create("builtin://texture/skybox/hdr/newport_loft.hdr");
		tex->load();
		//tex = Texture::Create("builtin://texture/skybox/hdr/newport_loft.hdr?type=irradiance");
		//tex->load();
		//tex = Texture::Create("builtin://texture/skybox/hdr/newport_loft.hdr?type=prefilter");
		//tex->load();

		tex = Texture::Create("builtin://texture/skybox/hdr/spree_bank_4k.hdr");
		tex->load();
		//tex = Texture::Create("builtin://texture/skybox/hdr/spree_bank_4k.hdr?type=irradiance");
		//tex->load();
		//tex = Texture::Create("builtin://texture/skybox/hdr/spree_bank_4k.hdr?type=prefilter");
		//tex->load();
		//HDRTextureLoader::GenerateBRDFLUT();
	
	}

	std::string Texture::getPath()
	{
		if (std::dynamic_pointer_cast<BlobHolder>(m_creationParam))
		{
			return std::static_pointer_cast<BlobHolder>(m_creationParam)->getPath();
		}

		if (std::dynamic_pointer_cast<TextureCreationParam>(m_creationParam))
		{
			std::string path = "";
			auto creationParam = std::static_pointer_cast<TextureCreationParam>(m_creationParam);
			if (creationParam->isProc)
			{
				path = creationParam->procPath;
			}
			else
			{
				if (creationParam->m_type == TextureCreationParam::CreateTextureType::Create2DSRV)
				{
					path += "Type:Create2DSRV,";
				}
				path += "width:" + std::to_string(creationParam->width) + "height:" + std::to_string(creationParam->height);
				
			}
			return path;
		}

		//if (std::dynamic_pointer_cast<HdrCreationParma>(m_creationParam))
		//{
		//	auto creationParam = std::static_pointer_cast<HdrCreationParma>(m_creationParam);
		//	if (creationParam->m_blobHolder)
		//	{
		//		return creationParam->m_blobHolder->getPath() + "?type=" + HdrCreationParma::mapTypeToString(creationParam->m_type);
		//	}
		//}

		return "";
	}

	void Texture::bind(std::shared_ptr<TextureDesc> desc)
	{
		if (!desc)
		{
			return;
		}

		for (const auto& textureBindFlag : desc->textureBindFlag)
		{
			if (textureBindFlag.second)
			{
				switch (textureBindFlag.first)
				{
				case TextureBindFlag::BindVS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetShaderResources(desc->startSlot, 1, (void* const*)&m_shaderResourceView);
					break;
				case TextureBindFlag::BindPS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetShaderResources(desc->startSlot, 1, (void* const*)&m_shaderResourceView);
					break;
				case TextureBindFlag::BindGS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->GSSetShaderResources(desc->startSlot, 1, (void* const*)&m_shaderResourceView);
					break;
				case TextureBindFlag::BindHS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->HSSetShaderResources(desc->startSlot, 1, (void* const*)&m_shaderResourceView);
					break;
				case TextureBindFlag::BindDS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->DSSetShaderResources(desc->startSlot, 1, (void* const*)&m_shaderResourceView);
					break;
				case TextureBindFlag::BindCS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->CSSetShaderResources(desc->startSlot, 1, (void* const*)&m_shaderResourceView);
					break;
				}
			}
		}
	}

	void Texture::unBind(std::shared_ptr<TextureDesc> desc)
	{
		if (!desc)
		{
			return;
		}

		ID3D11ShaderResourceView* srv = nullptr;
		for (const auto& textureBindFlag : desc->textureBindFlag)
		{
			if (textureBindFlag.second)
			{
				switch (textureBindFlag.first)
				{
				case TextureBindFlag::BindVS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetShaderResources(desc->startSlot, 1, (void* const*)&srv);
					break;
				case TextureBindFlag::BindPS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetShaderResources(desc->startSlot, 1, (void* const*)&srv);
					break;
				case TextureBindFlag::BindGS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->GSSetShaderResources(desc->startSlot, 1, (void* const*)&srv);
					break;
				case TextureBindFlag::BindHS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->HSSetShaderResources(desc->startSlot, 1, (void* const*)&srv);
					break;
				case TextureBindFlag::BindDS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->DSSetShaderResources(desc->startSlot, 1, (void* const*)&srv);
					break;
				case TextureBindFlag::BindCS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->CSSetShaderResources(desc->startSlot, 1, (void* const*)&srv);
					break;
				}
			}
		}
	}

	void Texture::updateBuffer(std::shared_ptr<Blob> data)
	{
		if (!data)
		{
			return;
		}

		D3D11_MAPPED_SUBRESOURCE mappedData;
		HRESULT hr = Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->Map(m_resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		if (SUCCEEDED(hr))
		{
			if (mappedData.RowPitch != data->getLength())
			{
				Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->Unmap(m_resource, 0);
				return;
			}

			memcpy_s(mappedData.pData, data->getLength(), data->getData(), data->getLength());
			Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->Unmap(m_resource, 0);
		}
	}

	bool Texture::getBuffer(std::shared_ptr<Blob>& data)
	{
		if (!data)
		{
			return false;
		}

		auto creationParam = std::dynamic_pointer_cast<TextureCreationParam>(m_creationParam);
		auto type = creationParam->m_type;
		if (!creationParam /* || (type != TextureCreationParam::CreateTextureType::CreateRaw && type != TextureCreationParam::CreateTextureType::CreateBuffer)*/)
		{
			return false;
		}

		unsigned int byteWidth = 0;

		switch (type)
		{
		case TextureCreationParam::CreateTextureType::None:
			break;
		case Destiny::TextureCreationParam::CreateTextureType::CreateBuffer:
			byteWidth = creationParam->bufferByteWidth;
			break;
		case TextureCreationParam::CreateTextureType::Create2DSRV:
			break;
		case TextureCreationParam::CreateTextureType::Create2DUAV:
			break;
		case TextureCreationParam::CreateTextureType::CreateStructured:
			byteWidth = creationParam->structuredBufferByteWidth;
			break;
		case TextureCreationParam::CreateTextureType::CreateTyped:
			break;
		case TextureCreationParam::CreateTextureType::CreateRaw:
			byteWidth = creationParam->rawBufferWidth;
			break;
		case TextureCreationParam::CreateTextureType::CreateTextureTypeCount:
			break;
		default:
			break;
		}

		D3D11_BUFFER_DESC stagingDesc = {};
		stagingDesc.ByteWidth = byteWidth;
		stagingDesc.Usage = D3D11_USAGE_STAGING;      
		stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; 
		stagingDesc.BindFlags = 0;                   
		stagingDesc.MiscFlags = 0;                   

		ID3D11Buffer* pStagingBuffer = nullptr;
		HRESULT hr = (HRESULT)Engine::GetInstance()->getGraphicsSystem()->getDevice()->CreateBuffer(&stagingDesc, nullptr, (void**)&pStagingBuffer);

		if (SUCCEEDED(hr))
		{
			Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->CopyResource(pStagingBuffer, m_resource);

			D3D11_MAPPED_SUBRESOURCE mappedData;
			hr = (HRESULT)Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->Map(pStagingBuffer, 0, D3D11_MAP_READ, 0, &mappedData);

			if (SUCCEEDED(hr))
			{
				if (mappedData.RowPitch < data->getLength())
				{
					SAFE_RELEASE(pStagingBuffer);
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->Unmap(m_resource, 0);
					return false;
				}

				memcpy_s(data->getData(), data->getLength(), mappedData.pData, data->getLength());
				Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->Unmap(m_resource, 0);

				SAFE_RELEASE(pStagingBuffer);
				return true;
			}
		}

		Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->Unmap(m_resource, 0);
		SAFE_RELEASE(pStagingBuffer);
		return false;
	}
}