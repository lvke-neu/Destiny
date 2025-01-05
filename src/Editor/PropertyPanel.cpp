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
			reflectProperty(property, object);
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

void PropertyPanel::reflectProperty(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	if (property.get_type() == rttr::type::get<std::string>())
	{
		reflectString(property, object);
	}
	else if (property.get_type() == rttr::type::get<float>())
	{
		reflectFloat(property, object);
	}
}

void PropertyPanel::reflectString(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	auto value = property.get_value(object).to_string();
	if (value.size() > MAX_BUFFER_SIZE)
	{
		return;
	}
	char buffer[MAX_BUFFER_SIZE];
	memset(buffer, 0, MAX_BUFFER_SIZE);
	memcpy_s(buffer, value.size(), value.c_str(), value.size());
	if (ImGui::InputText(property.get_name().data(), buffer, MAX_BUFFER_SIZE))
	{
		value = buffer;
		property.set_value(object, value);
	}
}

void PropertyPanel::reflectFloat(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	auto value = property.get_value(object).to_float();

	ImGui::PushID(property.get_name().data());
	ImGui::Columns(2);
	ImGui::Text(property.get_name().data());
	ImGui::NextColumn();
	if (ImGui::DragFloat("##", &value))
	{
		property.set_value(object, value);
	}
	ImGui::Columns(1);
	ImGui::PopID();


	//float step = 1.0f;
	//float columnWidth = 100.0f;

	//ImGuiIO& io = ImGui::GetIO();
	//auto boldFont = io.Fonts->Fonts[0];

	//ImGui::PushID(property.get_name().data());

	//ImGui::Columns(2);
	//ImGui::SetColumnWidth(0, columnWidth);
	//ImGui::Text(property.get_name().data());
	//ImGui::NextColumn();

	//ImGui::PushItemWidth(ImGui::CalcItemWidth());
	//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

	//float lineHeigh = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
	//ImVec2 buttonSize = { lineHeigh + 3.0f, lineHeigh };

	//ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.15f, 1.0f });
	//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.2f, 0.2f, 1.0f });
	//ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.1f, 0.15f, 1.0f });
	//ImGui::PushFont(boldFont);
	//if (ImGui::Button("X", buttonSize))
	//{
	//	value = 0.0f;
	//}
	//ImGui::PopFont();
	//ImGui::PopStyleColor(3);
	//ImGui::SameLine();
	//ImGui::DragFloat("##X", &value, step, 0.0f, 0.0f, "%.2f");
	//ImGui::PopStyleVar();
	//ImGui::PopItemWidth();
	//ImGui::Columns(1);

	//ImGui::PopID();
}

void PropertyPanel::onChoosedNode(void* parameter)
{
	m_choosedNode = *(std::shared_ptr<Destiny::Node>*) parameter;
}
