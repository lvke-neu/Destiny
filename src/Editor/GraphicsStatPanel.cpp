#include "GraphicsStatPanel.h"
#include "Imgui/imgui.h"
#include "ImGui/ImGuizmo.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/RenderTargetView.h"
#include "Graphics/BindRenderTargets.h"
#include "Graphics/VisualScene.h"
#include "Graphics/CameraComponent.h"
#include "Scene/SceneManager.h"

GraphicsStatPanel::GraphicsStatPanel()
{

}

void GraphicsStatPanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("GraphicsStat");

	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//ImGui::SameLine();
	ImGui::Text("DrawCallCount:%ld", Destiny::Engine::GetInstance()->getGraphicsSystem()->getGraphicsStat().DrawCallCount);
	//ImGui::SameLine();
	ImGui::Text("TriangleCount:%ld", Destiny::Engine::GetInstance()->getGraphicsSystem()->getGraphicsStat().TriangleCount);
	//ImGui::SameLine();
	ImGui::Text("VisualCount:%ld", Destiny::Engine::GetInstance()->getGraphicsSystem()->getGraphicsStat().VisualCount);

	ImGui::End();
	ImGui::PopStyleVar();
}

	