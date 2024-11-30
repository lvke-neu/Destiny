#include "ScenePanel.h"
#include "Imgui/imgui.h"
#include "Engine/Engine.h"
#include "Scene/SceneManager.h"
#include "Scene/NCS/Scene.h"
#include "Scene/NCS/VisualScene.h"
#include <queue>

static void BFS(std::shared_ptr<Destiny::Node> node)
{
	if (!node)
	{
		return;
	}

	if (ImGui::TreeNodeEx(node->get_name().c_str()))
	{
		for (const auto& child : node->getChilds())
		{
			BFS(child);
		}

		ImGui::TreePop();
	}
}

void ScenePanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

	ImGui::Begin("SceneNode");

	//ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	//bool b_IsOpen = ImGui::TreeNodeEx("aaa", treeNodeFlags);

	BFS(Destiny::Engine::GetInstance()->getSceneManager()->getScene());

	auto scene = std::dynamic_pointer_cast<Destiny::VisualScene>(Destiny::Engine::GetInstance()->getSceneManager()->getScene());
	if (scene)
	{
		auto node = scene->findCameraNode();
		if (node)
		{
			ImGui::Text(node->get_transform().toString().c_str());
		}
	}

	ImGui::End();

	ImGui::PopStyleVar();
}
	