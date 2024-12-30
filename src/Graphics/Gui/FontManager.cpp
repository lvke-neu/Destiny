#include "FontManager.h"
#include "Engine/Engine.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobLoader.h"
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
	}

	FontManager::~FontManager()
	{
		FT_Done_Face(m_face);
		FT_Done_FreeType(m_ft);
	}

	std::shared_ptr<Texture> FontManager::getFontTexture(char text, unsigned int width, unsigned int height)
	{
		FT_Set_Pixel_Sizes(m_face, width, height);

		if (FT_Load_Char(m_face, text, FT_LOAD_RENDER))
		{
			LOG_ERROR("ERROR::FREETYTPE: Failed to load Glyph:{0}", text);
			return nullptr;
		}
		std::shared_ptr<Texture> texture = nullptr;

		if (m_face->glyph && m_face->glyph->bitmap.pixel_mode == FT_RENDER_MODE_MONO)
		{
			std::vector<unsigned char> data;
			data.resize(width * height * 4, 255);
			texture = Texture::Create2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, data.data(), width * 4, width * 4 * height);
			texture->load(0);
		}

		return texture;
	}
}