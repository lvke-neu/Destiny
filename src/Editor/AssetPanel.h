#pragma once
#include <string>
#include <memory>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

namespace Destiny
{
	class Texture;
}

class AssetPanel
{
public:
	AssetPanel();
	~AssetPanel();
public:
	void update();
private:
	void drawContentBrowser();
private:
	std::string m_fullBuiltinPathString;
	std::experimental::filesystem::path m_fullBuiltinPath;
	std::shared_ptr<Destiny::Texture> m_directoryIcon;
	std::shared_ptr<Destiny::Texture> m_fileIcon;
	std::shared_ptr<Destiny::Texture> m_sceneIcon;
	std::shared_ptr<Destiny::Texture> m_hlslIcon;
	std::shared_ptr<Destiny::Texture> m_typefaceIcon;
	std::shared_ptr<Destiny::Texture> m_modelIcon;
};
