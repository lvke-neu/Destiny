#include "ComponentPanel.h"
#include "Imgui/imgui.h"

void ComponentPanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

	ImGui::Begin("Component");

	
	ImGui::End();

	ImGui::PopStyleVar();
}
	