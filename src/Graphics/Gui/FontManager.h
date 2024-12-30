#pragma once
#include "Engine/Utility.h"

struct FT_LibraryRec_;
struct FT_FaceRec_;
namespace Destiny
{
	class Texture;
	class FontManager
	{
		SINGLETON(FontManager);
	private:
		FontManager();
		~FontManager();
		FontManager(const FontManager&) = default;
		FontManager& operator=(const FontManager&) = default;
	public:
		std::shared_ptr<Texture> getFontTexture(char text, unsigned int width, unsigned int height);
	private:
		FT_LibraryRec_*		m_ft;
		FT_FaceRec_*		m_face;
	};
}