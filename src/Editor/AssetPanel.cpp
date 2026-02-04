#include "AssetPanel.h"
#include "Engine/Engine.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Graphics/Texture.h"
#include "Imgui/imgui.h"

AssetPanel::AssetPanel()
{
#if 0
	std::string pathPrefix = "builtin://";
    // ...
#endif
}

AssetPanel::~AssetPanel()
{

}

void AssetPanel::update()
{
#if 0
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    // ...
	ImGui::PopStyleVar();
#endif
}

void AssetPanel::drawContentBrowser()
{
#if 0
	if (std::experimental::filesystem::canonical(m_fullBuiltinPath) != std::experimental::filesystem::canonical(m_fullBuiltinPathString))
    // ...
#endif
}
	