#include "FontManager.h"
#include "Engine/Engine.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobLoader.h"
#include "Engine/Blob.h"
#include "Graphics/Texture.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <dxgiformat.h>

namespace Destiny
{
	FontManager::FontManager() :
		m_ft(nullptr),
		m_face(nullptr)
	{
		if (FT_Init_FreeType(&m_ft))
		{
			LOG_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
			return;
		} 

		const char* fontPath = "builtin://fonts/opensans/OpenSans-Regular.ttf";
		auto blobLoader = Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(fontPath);

		if (blobLoader)
		{
			auto blobHoder = blobLoader->createBlobHolder(fontPath);
			if (blobHoder)
			{
				if (FT_New_Face(m_ft, blobLoader->normalizedPath(blobHoder).c_str(), 0, &m_face))
				{
					LOG_ERROR("ERROR::FREETYPE: Failed to load font:{0}", fontPath);
					return;
				}
			}
		}

	
		std::vector<Color> color;
		for (int i = 0; i < 100; i++)
		{
			color.push_back({ 0,0,0,0 });
		}
		
		m_cache[' '].push_back(color);
	}

	FontManager::~FontManager()
	{
		FT_Done_Face(m_face);
		FT_Done_FreeType(m_ft);
	}

	std::shared_ptr<Texture> FontManager::getFontTexture(const std::string& text)
	{
		unsigned int maxHeight = 0;
		unsigned int width = 0;
		
		std::vector<std::vector<std::vector<Color>>> allArr;
		for (const auto& ch : text)
		{
			auto iter = m_cache.find(ch);
			if (iter == m_cache.end())
			{
				generateText(ch);
			}
			iter = m_cache.find(ch);

			width += (int)iter->second[0].size();
	
			allArr.push_back(iter->second);
			maxHeight = maxHeight > iter->second.size() ? maxHeight : (unsigned int)iter->second.size();
		}

		std::vector<Color> res;
		res.resize(width * maxHeight, { 0,0,0,0 });
		int offset = 0;

		for (unsigned int i = 0; i < maxHeight; i++)
		{
			for (const auto& arr : allArr)
			{
				if (i < arr.size())
				{
					memcpy_s(res.data() + offset, arr[i].size() * sizeof(int), arr[i].data(), arr[i].size() * sizeof(int));
					offset += (int)arr[i].size();
				}
				else
				{
					offset += (int)arr[0].size();
				}
			}
		}

		auto blobData = std::make_shared<Blob>(res.size() * sizeof(Color));
		blobData->copyfrom(res.data(), blobData->getLength());
		auto texture = Texture::Create2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, maxHeight, blobData, width * sizeof(Color), width * sizeof(Color) * maxHeight);
		texture->load(0);

		return texture;
	}

	void FontManager::generateText(unsigned char ch)
	{
		FT_Set_Pixel_Sizes(m_face, 0, 500);

		FT_Load_Char(m_face, ch, FT_LOAD_RENDER);
		auto bitMap = m_face->glyph->bitmap;
		m_cache[ch].resize(bitMap.rows);

		for (int i = 0; i < m_cache[ch].size(); i++)
		{
			m_cache[ch][i].resize(bitMap.width);
			for (int j = 0; j < m_cache[ch][i].size(); j++)
			{
				unsigned char pixel = *(bitMap.buffer + i * bitMap.width + j);
				m_cache[ch][i][j] = { pixel, pixel, pixel, pixel };
			}
		}
	}
}