#pragma once
#include "Engine/Asset.h"
#include <unordered_map>

struct ID3D11Resource;
struct ID3D11ShaderResourceView;
namespace Destiny
{
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
	private:
		ID3D11Resource* m_resource;
		ID3D11ShaderResourceView* m_shaderResourceView;
		static std::unordered_map<std::string, std::shared_ptr<Texture>> m_cache;
	};

}