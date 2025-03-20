#include "AssetPanel.h"
#include "Engine/Engine.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Graphics/Texture.h"
#include "Imgui/imgui.h"

AssetPanel::AssetPanel()
{
	std::string pathPrefix = "builtin://";
	auto blobLoader = Destiny::Engine::GetInstance()->getBlobLoaderManager()->getBlobLoader(pathPrefix.c_str());
	m_fullBuiltinPathString = blobLoader->normalizedPath(blobLoader->createBlobHolder(pathPrefix));
	m_fullBuiltinPath = m_fullBuiltinPathString;

	m_directoryIcon = Destiny::Texture::Create("builtin://texture/editor/directory.png");
	m_fileIcon = Destiny::Texture::Create("builtin://texture/editor/file.png");
	m_sceneIcon = Destiny::Texture::Create("builtin://texture/editor/scene.png");
	m_hlslIcon = Destiny::Texture::Create("builtin://texture/editor/hlsl.png");
	m_typefaceIcon = Destiny::Texture::Create("builtin://texture/editor/typeface.png");
	m_modelIcon = Destiny::Texture::Create("builtin://texture/editor/model.png");
}

AssetPanel::~AssetPanel()
{

}

void AssetPanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

	ImGui::Begin("Asset");

	drawContentBrowser();
	
	ImGui::End();

	ImGui::PopStyleVar();
}

void AssetPanel::drawContentBrowser()
{
	if (std::experimental::filesystem::canonical(m_fullBuiltinPath) != std::experimental::filesystem::canonical(m_fullBuiltinPathString))
	{
		if (ImGui::Button("<---")) 
		{
			m_fullBuiltinPath = m_fullBuiltinPath.parent_path();
		}
	}

	static float padding = 16.0f;
	static float thumbnailSize = 70.0f;
	float cellSize = thumbnailSize + padding;
	static float buttonSize = 50.0f;
	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
	{
		columnCount = 1;
	}
	
	ImGui::Columns(columnCount, 0, false);
	for (const auto& dir : std::experimental::filesystem::directory_iterator(m_fullBuiltinPath))
	{
		ImGui::PushID(std::experimental::filesystem::absolute(dir).u8string().c_str());

		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });

		std::shared_ptr<Destiny::Texture> texture = nullptr;
		if (std::experimental::filesystem::is_directory(dir))
		{
			texture = m_directoryIcon;
		}
		else
		{
			auto extension = dir.path().extension();
			if (extension == ".png" || extension == ".dds" || extension == ".jpg")
			{
				std::string imagePath = dir.path().u8string();
				auto pos = imagePath.find("builtin");
				if (pos != std::string::npos)
				{
					imagePath = imagePath.substr(pos + 8, imagePath.size() - pos - 8);
					imagePath = "builtin://" + imagePath;
				}
				texture = Destiny::Texture::Create(imagePath.c_str());
			}
			else if (extension == ".scene")
			{
				texture = m_sceneIcon;
			}
			else if (extension == ".hlsl")
			{
				texture = m_hlslIcon;
			}
			else if (extension == ".ttf")
			{
				texture = m_typefaceIcon;
			}
			else if (extension == ".obj" || extension == ".dae" || extension == ".gltf")
			{
				texture = m_modelIcon;
			}
			else
			{
				texture = m_fileIcon;
			}
		}
		
		if (texture)
		{
			if (texture->isLoadingPending())
			{
				texture->load();
			}
			ImGui::ImageButton(texture->getShaderResourceView(), { buttonSize, buttonSize });

			if (!std::experimental::filesystem::is_directory(dir))
			{
				if (ImGui::BeginDragDropSource())
				{
					std::string dragDropPath = dir.path().u8string();
					auto pos = dragDropPath.find("builtin");
					if (pos != std::string::npos)
					{
						dragDropPath = dragDropPath.substr(pos + 8, dragDropPath.size() - pos - 8);
						dragDropPath = "builtin://" + dragDropPath;
						std::replace(dragDropPath.begin(), dragDropPath.end(), '\\', '/');
					}

					ImGui::SetDragDropPayload("ASSET_BROWSER_ITEM", dragDropPath.c_str(), dragDropPath.size() + 1, ImGuiCond_Once);
					ImGui::Image(texture->getShaderResourceView(), { 30.0f, 30.0f });
					ImGui::SameLine();
					ImGui::Text("%s", dragDropPath.c_str());
					ImGui::EndDragDropSource();
				}
			}
		}
		
		ImGui::PopStyleColor();

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (std::experimental::filesystem::is_directory(dir))
			{
				m_fullBuiltinPath /= dir.path().filename();
			}
		}

		ImGui::TextWrapped(dir.path().filename().u8string().c_str());
		ImGui::NextColumn();

		ImGui::PopID();
	}
	ImGui::Columns(1);

	//ImGui::SliderFloat("Button Size", &buttonSize, 16, 512);
	//ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
	//ImGui::SliderFloat("Padding", &padding, 0, 32);
}
	