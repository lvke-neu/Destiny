#include "FontManager.h"
#include "Engine/Engine.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobLoader.h"
#include "Engine/Blob.h"
#include "Graphics/Texture.h"
#include "Math/Color.h"
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
	}

	FontManager::~FontManager()
	{
		FT_Done_Face(m_face);
		FT_Done_FreeType(m_ft);
	}

	std::shared_ptr<Texture> FontManager::getFontTexture(char text, unsigned int height)
	{
		auto iter = m_cache.find(text);
		if (iter != m_cache.end())
		{
			return iter->second;
		}

		FT_Set_Pixel_Sizes(m_face, 0, height);

		if (FT_Load_Char(m_face, text, FT_LOAD_RENDER))
		{
			LOG_ERROR("ERROR::FREETYTPE: Failed to load Glyph:{0}", text);
			return nullptr;
		}
		std::shared_ptr<Texture> texture = nullptr;

		if (m_face->glyph)
		{
			auto bitMap = m_face->glyph->bitmap;
			if (bitMap.pixel_mode == FT_RENDER_MODE_MONO)
			{
				std::vector<Pixel> data;
				data.resize(bitMap.width * bitMap.rows, { 0,0,0,0 });

				for (unsigned int i = 0; i < bitMap.rows; i++)
				{
					for (unsigned int j = 0; j < bitMap.width; j++)
					{
						unsigned char pixel = *(bitMap.buffer + i * bitMap.width + j);
						data[i * bitMap.width + j] = { pixel, pixel, pixel, pixel };
					}
				}

				auto blobData = std::make_shared<Blob>(data.size() * sizeof(Pixel));
				blobData->copyfrom(data.data(), blobData->getLength());
				texture = Texture::Create2D(DXGI_FORMAT_R8G8B8A8_UNORM, bitMap.width, bitMap.rows, blobData, bitMap.width * sizeof(Pixel), bitMap.width * sizeof(Pixel) * bitMap.rows);
				texture->load(0);

				m_cache[text] = texture;
			}
		}

		return texture;
	}
}