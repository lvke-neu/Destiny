#include "AssetPanel.h"
#include "Imgui/imgui.h"

void AssetPanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

	ImGui::Begin("Asset");

	
	ImGui::End();

	ImGui::PopStyleVar();
}
	