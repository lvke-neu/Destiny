#include "StatPanel.h"
#include "Imgui/imgui.h"
#include "Engine/Engine.h"
#include "Graphics/GraphicsSystem.h"

void StatPanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

	ImGui::Begin("Stat");

	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("DrawCallCount:%ld", Destiny::Engine::GetInstance()->getGraphicsSystem()->getGraphicsStat().DrawCallCount);

	ImGui::End();

	ImGui::PopStyleVar();
}
	