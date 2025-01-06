#include "PropertyPanel.h"
#include "Engine/Node.h"
#include "Engine/Component.h"
#include "Math/Color32.h"
#include "Math/Transform.h"
#include "Imgui/imgui.h"
#include "ImGui/imgui_internal.h"

PropertyPanel::PropertyPanel() :
	m_choosedNode(nullptr)
{
}

void PropertyPanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Property");

	reflect(m_choosedNode);

	if (m_choosedNode)
	{
		for (const auto& componet : m_choosedNode->getComponents())
		{
			reflect(componet);
		}
	}

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

		//if (std::dynamic_pointer_cast<Destiny::Node>(object))
		//{
		//	for (const auto& componet : std::dynamic_pointer_cast<Destiny::Node>(object)->getComponents())
		//	{
		//		reflect(componet);
		//	}
		//}
		ImGui::TreePop();
	}
	ImGui::PopID();

	ImGui::PopStyleVar();
}

void PropertyPanel::reflectProperty(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	if (property.get_type() == rttr::type::get<bool>())
	{
		reflectBool(property, object);
	}
	else if (property.get_type() == rttr::type::get<std::string>())
	{
		reflectString(property, object);
	}
	else if (property.get_type() == rttr::type::get<float>())
	{
		reflectFloat(property, object);
	}
	else if (property.get_type() == rttr::type::get<DirectX::XMFLOAT2>())
	{
		reflectFloat2(property, object);
	}
	else if (property.get_type() == rttr::type::get<DirectX::XMFLOAT3>())
	{
		reflectFloat3(property, object);
	}
	else if (property.get_type() == rttr::type::get<Destiny::Color32>())
	{
		reflectColor(property, object);
	}
	else if (property.get_type() == rttr::type::get<Destiny::Transform>())
	{
		reflectTransform(property, object);
	}
}

void PropertyPanel::reflectBool(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	auto value = property.get_value(object).to_bool();
	if (ImGui::Checkbox(property.get_name().data(), &value))
	{
		property.set_value(object, value);
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
}

void PropertyPanel::reflectFloat2(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	bool changed = false;

	DirectX::XMFLOAT2 value;
	property.get_value(object).convert(value);

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(property.get_name().data());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 100.0f);
	ImGui::Text(property.get_name().data());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

	float lineHeigh = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeigh + 3.0f, lineHeigh };

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
	{
		value.x = 0.0f;
		changed = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	if (ImGui::DragFloat("##X", &value.x))
	{
		changed = true;
	}
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		value.y = 0.0f;
		changed = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	if (ImGui::DragFloat("##Y", &value.y))
	{
		changed = true;
	}
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();
	
	ImGui::Columns(1);
	ImGui::PopID();

	if (changed)
	{
		property.set_value(object, value);
	}
}

void PropertyPanel::reflectFloat3(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	bool changed = false;

	DirectX::XMFLOAT3 value;
	property.get_value(object).convert(value);

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(property.get_name().data());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 100.0f);
	ImGui::Text(property.get_name().data());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

	float lineHeigh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeigh + 3.0f, lineHeigh };

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
	{
		value.x = 0.0f;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	if (ImGui::DragFloat("##X", &value.x))
	{
		changed = true;
	}
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		value.y = 0.0f;
		changed = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	if (ImGui::DragFloat("##Y", &value.y))
	{
		changed = true;
	}
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.2f, 0.3f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
	{
		value.z = 0.0f;
		changed = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	if (ImGui::DragFloat("##Z", &value.z))
	{
		changed = true;
	}
	ImGui::PopItemWidth();


	ImGui::PopStyleVar();

	ImGui::Columns(1);
	ImGui::PopID();

	if (changed)
	{
		property.set_value(object, value);
	}
}

bool reflectTransformFloat3(const rttr::property& property, DirectX::XMFLOAT3& value)
{
	bool changed = false;

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(property.get_name().data());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 100.0f);
	ImGui::Text(property.get_name().data());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

	float lineHeigh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeigh + 3.0f, lineHeigh };

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
	{
		value.x = 0.0f;
		changed = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	if (ImGui::DragFloat("##X", &value.x))
	{
		changed = true;
	}
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		value.y = 0.0f;
		changed = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	if (ImGui::DragFloat("##Y", &value.y))
	{
		changed = true;
	}
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.2f, 0.3f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
	{
		value.z = 0.0f;
		changed = true;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);
	ImGui::SameLine();
	if (ImGui::DragFloat("##Z", &value.z))
	{
		changed = true;
	}
	ImGui::PopItemWidth();


	ImGui::PopStyleVar();

	ImGui::Columns(1);
	ImGui::PopID();

	return changed;
}

void PropertyPanel::reflectColor(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	Destiny::Color32 value;
	property.get_value(object).convert(value);

	if (ImGui::ColorEdit4(property.get_name().data(), (float*)&value, ImGuiColorEditFlags_Float))
	{
		property.set_value(object, value);
	}
}

void PropertyPanel::reflectTransform(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	Destiny::Transform value;
	property.get_value(object).convert(value);

	auto translation = value.get_translation();
	auto rotation = value.get_rotation();
	auto scale = value.get_scale();

	auto type = rttr::type::get<Destiny::Transform>();
	bool changed = false;
	if (reflectTransformFloat3(type.get_property("translation"), translation))
	{
		changed = true;
		value.set_translation(translation);
	}
	
	if (reflectTransformFloat3(type.get_property("rotation"), rotation))
	{
		changed = true;
		value.set_rotation(rotation);
	}

	if (reflectTransformFloat3(type.get_property("scale"), scale))
	{
		changed = true;
		value.set_scale(scale);
	}

	if (changed)
	{
		property.set_value(object, value);
	}

}

void PropertyPanel::onChoosedNode(void* parameter)
{
	m_choosedNode = *(std::shared_ptr<Destiny::Node>*) parameter;
}
