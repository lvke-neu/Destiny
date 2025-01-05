#include "ScenePanel.h"
#include "Imgui/imgui.h"
#include "Engine/Engine.h"
#include "Engine/Node.h"
#include "Engine/Component.h"
#include "Graphics/VisualScene.h"
#include "Scene/SceneManager.h"
#include <queue>
#include <rttr/registration>
#include <rttr/type>

void ScenePanel::traversal(std::shared_ptr<Destiny::Node> node)
{
	if (!node)
	{
		return;
	}
	
	ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4,4 });
	ImGui::Separator();

	ImGui::PushID(node->get_uuid().c_str());
	if (ImGui::TreeNodeEx(node->get_name().c_str(), treeNodeFlags))
	{
		send(ChoosedNode, &node);

		for (const auto& child : node->getChilds())
		{
			traversal(child);
		}

		ImGui::TreePop();
	}
	ImGui::PopID();

	ImGui::PopStyleVar();
}

void ScenePanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

	ImGui::Begin("SceneNode");

	traversal(Destiny::Engine::GetInstance()->getSceneManager()->getScene());

	if (ImGui::BeginPopup("Components"))
	{
		//rttr::type base_type = rttr::type::get<Destiny::CameraComponent>();
		//auto aa = base_type.create();
		//auto cp = aa.get_value<Destiny::Component*>();

		//auto cp = derived_type.create().get_value<Destiny::CameraComponent*>();
		
		rttr::type base_type = rttr::type::get<Destiny::Component>();
		auto derived_types = base_type.get_derived_classes();
		for (const auto& derived_type : derived_types)
		{
			if (ImGui::Button(derived_type.get_name().data()))
			{
				auto variant = derived_type.create();
				auto component = variant.get_value<std::shared_ptr<Destiny::Component>>();
				auto node = std::make_shared<Destiny::Node>();
				node->set_name(derived_type.get_name().data());
				node->addComponent(component);
				node->addToParent(Destiny::Engine::GetInstance()->getSceneManager()->getScene());
			}
		}
		
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::Button("AddComponent"))
	{

		ImGui::OpenPopup("Components");
	}

	if (ImGui::Button("RemoveNode"))
	{
		auto childs = Destiny::Engine::GetInstance()->getSceneManager()->getScene()->getChilds();
		if (!childs.empty())
		{
			if (childs.back() != Destiny::Engine::GetInstance()->getSceneManager()->getScene()->getCameraNode())
			{
				childs.back()->removeFromParent();
			}
		}
	}

	ImGui::End();

	ImGui::PopStyleVar();
}
	