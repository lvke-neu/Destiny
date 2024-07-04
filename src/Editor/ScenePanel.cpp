#include "ScenePanel.h"
#include "Imgui/imgui.h"

void ScenePanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

	ImGui::Begin("SceneNode");

	
	ImGui::End();

	ImGui::PopStyleVar();
}
	