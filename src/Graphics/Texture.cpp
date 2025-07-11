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
		m_unorderedAccessView(nullptr)
	{

	}

	Texture::Texture(ID3D11Resource* resource, ID3D11ShaderResourceView* shaderResourceView) :
		m_resource(resource),
		m_shaderResourceView(shaderResourceView),
		m_unorderedAccessView(nullptr)
	{
		SAFE_ADDREF(m_resource);
		SAFE_ADDREF(m_shaderResourceView);
	}

	Texture::~Texture()
	{
		SAFE_RELEASE(m_resource);
		SAFE_RELEASE(m_shaderResourceView);
		SAFE_RELEASE(m_unorderedAccessView);
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

	std::shared_ptr<Texture> Texture::Create2DSRV(int format, unsigned int width, unsigned int height, std::shared_ptr<Blob> data, unsigned int pitch, unsigned int slicePitch, bool isProc, const std::string& procPath)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		std::shared_ptr<TextureCreationParam> creationParam = std::make_shared<TextureCreationParam>();
		
		creationParam->m_type = TextureCreationParam::Create2DSRV;
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

		creationParam->m_type = TextureCreationParam::Create2DUAV;
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
				if (creationParam->m_type == TextureCreationParam::Create2DSRV)
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
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetShaderResources(desc->startSlot, 1, &m_shaderResourceView);
					break;
				case TextureBindFlag::BindPS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetShaderResources(desc->startSlot, 1, &m_shaderResourceView);
					break;
				case TextureBindFlag::BindGS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->GSSetShaderResources(desc->startSlot, 1, &m_shaderResourceView);
					break;
				case TextureBindFlag::BindHS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->HSSetShaderResources(desc->startSlot, 1, &m_shaderResourceView);
					break;
				case TextureBindFlag::BindDS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->DSSetShaderResources(desc->startSlot, 1, &m_shaderResourceView);
					break;
				case TextureBindFlag::BindCS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->CSSetShaderResources(desc->startSlot, 1, &m_shaderResourceView);
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
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->VSSetShaderResources(desc->startSlot, 1, &srv);
					break;
				case TextureBindFlag::BindPS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->PSSetShaderResources(desc->startSlot, 1, &srv);
					break;
				case TextureBindFlag::BindGS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->GSSetShaderResources(desc->startSlot, 1, &srv);
					break;
				case TextureBindFlag::BindHS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->HSSetShaderResources(desc->startSlot, 1, &srv);
					break;
				case TextureBindFlag::BindDS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->DSSetShaderResources(desc->startSlot, 1, &srv);
					break;
				case TextureBindFlag::BindCS:
					Engine::GetInstance()->getGraphicsSystem()->getImmediateContext()->CSSetShaderResources(desc->startSlot, 1, &srv);
					break;
				}
			}
		}
	}
}