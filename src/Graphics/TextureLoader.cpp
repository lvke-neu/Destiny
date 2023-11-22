#include "TextureLoader.h"
#include "Engine/Engine.h"
#include "Engine/Blob.h"
#include "Engine/BlobHolder.h"
#include "Engine/Utility.h"
#include "Texture.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "GraphicsSystem.h"

namespace Destiny
{
	TextureLoader::TextureLoader()
	{
		ID3D11Resource* resource = nullptr;
		DirectX::CreateDDSTextureFromFile(Engine::GetInstance()->getGraphicsSystem()->getDevice(), L"", &resource, nullptr);
		DirectX::CreateWICTextureFromFile(Engine::GetInstance()->getGraphicsSystem()->getDevice(), L"", &resource, nullptr);
	}

	TextureLoader::~TextureLoader()
	{

	}

	void TextureLoader::loadAsset(std::shared_ptr<Asset> asset)
	{
		m_mtx.lock();
		if (!asset || !asset->getBlobHolder() || !asset->getBlobHolder()->getBlob())
		{
			asset->loadFailed__();
			m_mtx.unlock();
			return;
		}

		std::string path((char*)asset->getBlobHolder()->getBlob()->getData(), asset->getBlobHolder()->getBlob()->getLength());
		std::string tmpPath = path;
		size_t prefix = path.find("assets://");
		if (prefix == std::string::npos)
		{
			LOG_ERROR("Texture load failed : {0}", tmpPath);
			asset->loadFailed__();
			m_mtx.unlock();
			return;
		}
		path = path.substr(prefix + 9);

		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, sizeof(buffer));

		std::string exePath = buffer;
		auto pos = exePath.find("Destiny");
		if (pos == exePath.npos)
		{
			LOG_ERROR("Texture load failed : {0}", tmpPath);
			asset->loadFailed__();
			m_mtx.unlock();
			return;
		}
		exePath = exePath.substr(0, pos + 7);

		path = exePath + "\\assets\\" + path;

		if (path.substr(path.rfind(".") + 1, 3) == "dds")
		{
			HRESULT hr = DirectX::CreateDDSTextureFromFile(Engine::GetInstance()->getGraphicsSystem()->getDevice(), Utility::MultiByte2WideChar(path).c_str(), &((Texture*)asset.get())->m_resource, &((Texture*)asset.get())->m_shaderResourceView);
			if (SUCCEEDED(hr))
			{
				asset->loadSucceeded__();
			}
			else
			{
				LOG_ERROR("Texture load failed : {0}", tmpPath);
				asset->loadFailed__();
			}
		}
		else
		{
			HRESULT hr = DirectX::CreateWICTextureFromFile(Engine::GetInstance()->getGraphicsSystem()->getDevice(), Utility::MultiByte2WideChar(path).c_str(), &((Texture*)asset.get())->m_resource, &((Texture*)asset.get())->m_shaderResourceView);
			if (SUCCEEDED(hr))
			{
				asset->loadSucceeded__();
			}
			else
			{
				LOG_ERROR("Texture load failed : {0}", tmpPath);
				asset->loadFailed__();
			}
		}
		
		m_mtx.unlock();
	}

	std::shared_ptr<Texture> TextureLoader::createAsset(std::shared_ptr<BlobHolder> blobHolder)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->initialize(shared_from_this(), blobHolder);
		return texture;
	}
}