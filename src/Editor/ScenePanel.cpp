#include "ScenePanel.h"
#include "Imgui/imgui.h"
#include "Engine/Engine.h"
#include "Engine/Node.h"
#include "Engine/Component.h"
#include "Engine/Serializer.h"
#include "Engine/UnSerializer.h"
#include "Graphics/VisualScene.h"
#include "Scene/SceneManager.h"
#include <queue>
#include <rttr/registration>
#include <rttr/type>

ScenePanel::ScenePanel() :
	m_choosedNode(nullptr)
{

}

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

	if (ImGui::TreeNodeEx("##TreeNode", treeNodeFlags))
	{
		m_choosedNode = node;
		send(ChoosedNode, &node);
		popup();

		ImGui::SameLine();
		ImGui::Text(node->get_name().c_str());

		for (const auto& child : node->getChilds())
		{
			traversal(child);
		}

		ImGui::TreePop();
	}
	else
	{
		m_choosedNode = node;
		popup();
		ImGui::SameLine();
		ImGui::Text(node->get_name().c_str());
	}
	ImGui::PopID();

	ImGui::PopStyleVar();
}

void ScenePanel::popup()
{
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
		ImGui::OpenPopup("NodeOperationPopup");
	}
	if (ImGui::BeginPopup("NodeOperationPopup")) 
	{

		//if (ImGui::Button("Rename"))
		//{
		//	if (m_choosedNode)
		//	{
		//		ImGui::OpenPopup("Rename");
		//	}	
		//}
		//if (ImGui::BeginPopup("Rename"))
		//{
		//	char buffer[MAX_BUFFER_SIZE];
		//	memset(buffer, 0, MAX_BUFFER_SIZE);
		//	memcpy_s(buffer, m_choosedNode->get_name().size(), m_choosedNode->get_name().c_str(), m_choosedNode->get_name().size());
		//	
		//	if (ImGui::InputText("Node Name", buffer, MAX_BUFFER_SIZE))
		//	{
		//		m_choosedNode->set_name(buffer);
		//	}

		//	if (ImGui::Button("OK"))
		//	{
		//		ImGui::CloseCurrentPopup(); 
		//	}

		//	ImGui::EndPopup();
		//}
		ImGui::Text((m_choosedNode->get_name() + ":").c_str());
		if (ImGui::Button("Locate"))
		{
			if (m_choosedNode)
			{
				auto scene = Destiny::Engine::GetInstance()->getSceneManager()->getCurrentScene();
				if (scene)
				{
					Destiny::Transform transform;
					transform.set_translation(m_choosedNode->get_translation());
					scene->locate(transform);
				}
			}
		}
		if (ImGui::Button("AddNode")) 
		{
			auto node = std::make_shared<Destiny::Node>("New Node");
			node->addToParent(m_choosedNode);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("RemoveNode")) 
		{
			if (m_choosedNode)
			{
				m_choosedNode->removeFromParent();
				send(ChoosedNode, nullptr);
			}
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("CopyNode"))
		{
			std::string jsonStr;
			Destiny::Serializer::Serialize(jsonStr, m_choosedNode);
			Destiny::Utility::CopyToClipboard(jsonStr);
		}
		if (ImGui::Button("PasteNode"))
		{
			std::string jsonStr = Destiny::Utility::GetClipboardText();
			std::shared_ptr<Destiny::Object> object = nullptr;
			Destiny::UnSerializer::UnSerialize(object, jsonStr);

			std::shared_ptr<Destiny::Node> node = std::dynamic_pointer_cast<Destiny::Node>(object);
			if (node)
			{
				node->addToParent(m_choosedNode);
			}
		}
		if (ImGui::Button("AddComponent"))
		{
			ImGui::OpenPopup("AddComponent");
		}
		if (ImGui::BeginPopup("AddComponent"))
		{
			rttr::type base_type = rttr::type::get<Destiny::Component>();
			auto derived_types = base_type.get_derived_classes();
			
			std::vector<rttr::type> derivedTypes(derived_types.begin(), derived_types.end());

			auto compareFunction = 
				[](const rttr::type& a, const rttr::type& b) 
				{
					return a.get_name().to_string()[0] < b.get_name().to_string()[0];
				};
			std::sort(derivedTypes.begin(), derivedTypes.end(), compareFunction);

			for (const auto& derived_type : derivedTypes)
			{
				if (ImGui::Button(derived_type.get_name().data()))
				{
					if (m_choosedNode)
					{
						auto variant = derived_type.create();
						auto component = variant.get_value<std::shared_ptr<Destiny::Component>>();
						m_choosedNode->addComponent(component);
					}

					ImGui::CloseCurrentPopup();
				}
			}
	
			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::EndPopup();
	}
}

void ScenePanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

	ImGui::Begin("SceneNode");

	traversal(Destiny::Engine::GetInstance()->getSceneManager()->getCurrentScene());

	ImGui::End();

	ImGui::PopStyleVar();
}
	