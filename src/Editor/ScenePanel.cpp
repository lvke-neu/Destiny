#include "ScenePanel.h"
#include "Imgui/imgui.h"
#include "Engine/Engine.h"
#include "Engine/Node.h"
#include "Graphics/VisualScene.h"
#include "Scene/SceneManager.h"
#include <queue>

void ScenePanel::traversal(std::shared_ptr<Destiny::Node> node)
{
	if (!node)
	{
		return;
	}
	
	ImGui::PushID(node->get_uuid().c_str());
	if (ImGui::TreeNodeEx(node->get_name().c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected))
	{
		send(ChoosedNode, &node);

		for (const auto& child : node->getChilds())
		{
			traversal(child);
		}

		ImGui::TreePop();
	}
	ImGui::PopID();
}

void ScenePanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

	ImGui::Begin("SceneNode");

	traversal(Destiny::Engine::GetInstance()->getSceneManager()->getScene());

	ImGui::End();

	ImGui::PopStyleVar();
}
	