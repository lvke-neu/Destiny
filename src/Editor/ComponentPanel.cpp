#include "ComponentPanel.h"
#include "Imgui/imgui.h"

void ComponentPanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

	ImGui::Begin("Component");

	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();

	ImGui::PopStyleVar();
}
	