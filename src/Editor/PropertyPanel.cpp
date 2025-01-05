#include "PropertyPanel.h"
#include "Engine/Node.h"
#include "Engine/Component.h"
#include "Imgui/imgui.h"


PropertyPanel::PropertyPanel() :
	m_choosedNode(nullptr)
{
}

void PropertyPanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Property");

	reflect(m_choosedNode);

	ImGui::End();
	ImGui::PopStyleVar();
}

void PropertyPanel::reflect(std::shared_ptr<Destiny::Object> object)
{
	if (!object)
	{
		return;
	}

	auto type = rttr::type::get(*object.get());

	ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4,4 });
	ImGui::Separator();

	ImGui::PushID(object->get_uuid().c_str());
	if (ImGui::TreeNodeEx(type.get_name().data(), treeNodeFlags))
	{	
		for (const auto& property : type.get_properties())
		{
			ImGui::Text(property.get_name().data());
		}

		if (std::dynamic_pointer_cast<Destiny::Node>(object))
		{
			for (const auto& componet : std::dynamic_pointer_cast<Destiny::Node>(object)->getComponents())
			{
				reflect(componet);
			}
		}
		ImGui::TreePop();
	}
	ImGui::PopID();

	ImGui::PopStyleVar();
}

void PropertyPanel::onChoosedNode(void* parameter)
{
	m_choosedNode = *(std::shared_ptr<Destiny::Node>*) parameter;
}
