#include "PropertyPanel.h"
#include "Engine/Node.h"
#include "Engine/Component.h"
#include "Engine/Utility.h"
#include "Engine/Serializer.h"
#include "Engine/UnSerializer.h"
#include "Math/Color.h"
#include "Math/Transform.h"
#include "Graphics/GraphicsDefine.h"
#include "Graphics/PbrMaterial.h"
#include "Graphics/VisualComponent.h"
#include "Imgui/imgui.h"
#include "ImGui/imgui_internal.h"
#include <d3d11.h>

PropertyPanel::PropertyPanel() :
	m_choosedNode(nullptr),
	m_dragFloatStep(1.0f)
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

void PropertyPanel::setDragFloatStep(float dragFloatStep)
{
	m_dragFloatStep = dragFloatStep;
}

void PropertyPanel::popup(std::shared_ptr<Destiny::Component> component)
{
	if (!component)
	{
		return;
	}

	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) 
	{
		ImGui::OpenPopup("ComponentOperationPopup");
	}

	if (ImGui::BeginPopup("ComponentOperationPopup"))
	{
		if (ImGui::Button(("Remove " + rttr::type::get(*component.get()).get_name().to_string()).c_str()))
		{
			m_choosedNode->removeComponent(component);
		}
	
		ImGui::EndPopup();
	}
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
		popup(std::dynamic_pointer_cast<Destiny::Component>(object));

		for (const auto& property : type.get_properties())
		{
			reflectProperty(property, object);
			ImGui::Separator();
		}
		
		ImGui::TreePop();

		//if (ImGui::Button("Serialize And UnSerialize"))
		//{
		//	std::string jsonStr;
		//	Destiny::Serializer::Serialize(jsonStr, object);
		//	using namespace Destiny;
		//	LOG_WARN(jsonStr);

		//	std::shared_ptr<Destiny::Object> object;
		//	Destiny::UnSerializer::UnSerialize(object, jsonStr);
		//}
	}
	else
	{
		popup(std::dynamic_pointer_cast<Destiny::Component>(object));
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
	else if (property.get_type() == rttr::type::get<Destiny::Color>())
	{
		reflectColor(property, object);
	}
	else if (property.get_type() == rttr::type::get<Destiny::Transform>())
	{
		reflectTransform(property, object);
	}
	else if (property.get_type().is_enumeration())
	{
		reflectEnumeration(property, object);
	}
	else if (property.get_type() == rttr::type::get<D3D11_RASTERIZER_DESC>())
	{
		reflectRasterizerDesc(property, object);
	}	
	else if (property.get_type() == rttr::type::get<D3D11_DEPTH_STENCIL_DESC>())
	{
		reflectDepthStencilStateDesc(property, object);
	}
	else if (property.get_type() == rttr::type::get<D3D11_BLEND_DESC>())
	{
		reflectBlendDesc(property, object);
	}
	else if (property.get_type() == rttr::type::get<Destiny::Button>())
	{
		reflectButton(property, object);
	}
	else if (property.get_type() == rttr::type::get<std::shared_ptr<Destiny::Material>>())
	{
		std::shared_ptr<Destiny::Material> material = nullptr;
		property.get_value(object).convert(material);

		if (!material)
		{
			if (ImGui::BeginPopup("AddMaterial"))
			{
				rttr::type base_type = rttr::type::get<Destiny::Material>();
				auto derived_types = base_type.get_derived_classes();
				for (const auto& derived_type : derived_types)
				{
					if (ImGui::Button(derived_type.get_name().data()))
					{
						auto variant = derived_type.create();
						auto material = variant.get_value<std::shared_ptr<Destiny::Material>>();

						if (std::dynamic_pointer_cast<Destiny::VisualComponent>(object))
						{
							std::dynamic_pointer_cast<Destiny::VisualComponent>(object)->set_material(material);
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

			if (ImGui::Button("AddMaterial"))
			{
				ImGui::OpenPopup("AddMaterial");
			}
		}

		reflectMaterial(material);
	}
}

void PropertyPanel::reflectBool(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	auto value = property.get_value(object).to_bool();
	ImGui::Text((property.get_name().to_string() + " :").c_str());
	ImGui::SameLine();
	if (ImGui::Checkbox(("##" + property.get_name().to_string()).c_str(), &value))
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
	ImGui::Text((property.get_name().to_string() + " :").c_str());
	ImGui::SameLine();
	if (ImGui::InputText(("##" + property.get_name().to_string()).c_str(), buffer, MAX_BUFFER_SIZE))
	{
		value = buffer;
		property.set_value(object, value);
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_BROWSER_ITEM"))
		{
			value = (char*)payload->Data;
			property.set_value(object, value);
		}
		ImGui::EndDragDropTarget();
	}

	if (property.get_name().to_string() == "renderer")
	{
		if (ImGui::Button("ReCompile Shader"))
		{
			std::static_pointer_cast<Destiny::VisualComponent>(object)->reCompileShader();
		}
		ImGui::SameLine();
		//if (ImGui::Button("Editor Shader"))
		//{
		//	//ImGui::OpenPopup("EditorShader");

		//	//if (ImGui::BeginPopup("EditorShader"))
		//	//{
		//	//	ImGui::Text("sdsadsads");
		//	//	if (ImGui::Button("Close"))
		//	//	{
		//	//		ImGui::CloseCurrentPopup();
		//	//	}
		//	//	ImGui::EndPopup();
		//	//}
		//}
	}
}

void PropertyPanel::reflectFloat(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	auto value = property.get_value(object).to_float();

	ImGui::PushID(property.get_name().data());
	ImGui::Columns(2);
	ImGui::Text(property.get_name().data());
	ImGui::NextColumn();
	if (ImGui::DragFloat(("##" + property.get_name().to_string()).c_str(), &value, m_dragFloatStep, 0.0f, 0.0f, "%.6f"))
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
	if (ImGui::DragFloat("##X", &value.x, m_dragFloatStep, 0.0f, 0.0f, "%.6f"))
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
	if (ImGui::DragFloat("##Y", &value.y, m_dragFloatStep, 0.0f, 0.0f, "%.6f"))
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
	if (ImGui::DragFloat("##X", &value.x, m_dragFloatStep, 0.0f, 0.0f, "%.6f"))
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
	if (ImGui::DragFloat("##Y", &value.y, m_dragFloatStep, 0.0f, 0.0f, "%.6f"))
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
	if (ImGui::DragFloat("##Z", &value.z, m_dragFloatStep, 0.0f, 0.0f, "%.6f"))
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

void PropertyPanel::reflectColor(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	Destiny::Color value;
	property.get_value(object).convert(value);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
	ImGui::Text((property.get_name().to_string() + " :").c_str());
	ImGui::SameLine();
	if (ImGui::ColorEdit4(("##" + property.get_name().to_string()).c_str(), (float*)&value, ImGuiColorEditFlags_Float))
	{
		property.set_value(object, value);
	}
	ImGui::PopStyleVar();
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
	if (reflectFloat3(type.get_property("translation"), translation))
	{
		changed = true;
		value.set_translation(translation);
	}
	
	if (reflectFloat3(type.get_property("rotation"), rotation))
	{
		changed = true;
		value.set_rotation(rotation);
	}

	if (reflectFloat3(type.get_property("scale"), scale))
	{
		changed = true;
		value.set_scale(scale);
	}

	if (changed)
	{
		property.set_value(object, value);
	}
}

void PropertyPanel::reflectEnumeration(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	auto enumeration = property.get_enumeration();

	std::vector<std::string> items;
	for (const auto& value : enumeration.get_values())
	{
		items.push_back(value.to_string());
	}

	ImGui::Text((property.get_name().to_string() + " :").c_str());
	ImGui::SameLine();

	//int itemIndex = property.get_value(object).to_int();
	auto itemIndex = (int)std::distance(items.begin(), std::find(items.begin(), items.end(), property.get_value(object).to_string()));
	if (ImGui::Combo(("##" + property.get_name().to_string()).c_str(), &itemIndex,
		[](void* data, int idx, const char** out_text)
		{
			auto& vector = *static_cast<std::vector<std::string>*>(data);
			if (idx < 0 || idx >= static_cast<int>(vector.size())) {
				return false;
			}
			*out_text = vector[idx].c_str();
			return true;
		},
		&items, (int)items.size()))
	{
		property.set_value(object, enumeration.name_to_value(items[itemIndex]));
	}
}

void PropertyPanel::reflectRasterizerDesc(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	bool changed = false;

	D3D11_RASTERIZER_DESC desc;
	property.get_value(object).convert(desc);

	auto type = rttr::type::get<D3D11_RASTERIZER_DESC>();

	auto FillMode = (int)desc.FillMode;
	if (reflectEnumeration(type.get_property("FillMode"), FillMode))
	{
		desc.FillMode = (D3D11_FILL_MODE)FillMode;
		changed = true;
	}

	auto CullMode = (int)desc.CullMode;
	if (reflectEnumeration(type.get_property("CullMode"), CullMode))
	{
		desc.CullMode = (D3D11_CULL_MODE)CullMode;
		changed = true;
	}

	auto FrontCounterClockwise = (bool)desc.FrontCounterClockwise;
	if (reflectBool(type.get_property("FrontCounterClockwise"), FrontCounterClockwise))
	{
		desc.FrontCounterClockwise = FrontCounterClockwise;
		changed = true;
	}

	auto DepthBias = desc.DepthBias;
	if (reflectInt(type.get_property("DepthBias"), DepthBias))
	{
		desc.DepthBias = DepthBias;
		changed = true;
	}
	
	auto DepthBiasClamp = desc.DepthBiasClamp;
	if (reflectFloat(type.get_property("DepthBiasClamp"), DepthBiasClamp))
	{
		desc.DepthBiasClamp = DepthBiasClamp;
		changed = true;
	}

	auto SlopeScaledDepthBias = desc.SlopeScaledDepthBias;
	if (reflectFloat(type.get_property("SlopeScaledDepthBias"), SlopeScaledDepthBias))
	{
		desc.SlopeScaledDepthBias = SlopeScaledDepthBias;
		changed = true;
	}

	auto DepthClipEnable = (bool)desc.DepthClipEnable;
	if (reflectBool(type.get_property("DepthClipEnable"), DepthClipEnable))
	{
		desc.DepthClipEnable = DepthClipEnable;
		changed = true;
	}

	auto ScissorEnable = (bool)desc.ScissorEnable;
	if (reflectBool(type.get_property("ScissorEnable"), ScissorEnable))
	{
		desc.ScissorEnable = ScissorEnable;
		changed = true;
	}

	auto MultisampleEnable = (bool)desc.MultisampleEnable;
	if (reflectBool(type.get_property("MultisampleEnable"), MultisampleEnable))
	{
		desc.MultisampleEnable = MultisampleEnable;
		changed = true;
	}

	auto AntialiasedLineEnable = (bool)desc.AntialiasedLineEnable;
	if (reflectBool(type.get_property("AntialiasedLineEnable"), AntialiasedLineEnable))
	{
		desc.AntialiasedLineEnable = AntialiasedLineEnable;
		changed = true;
	}

	if (changed)
	{
		property.set_value(object, desc);
	}
}

void PropertyPanel::reflectDepthStencilStateDesc(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	bool changed = false;

	D3D11_DEPTH_STENCIL_DESC desc;
	property.get_value(object).convert(desc);

	auto type = rttr::type::get<D3D11_DEPTH_STENCIL_DESC>();

	auto DepthEnable = (bool)desc.DepthEnable;
	if (reflectBool(type.get_property("DepthEnable"), DepthEnable))
	{
		desc.DepthEnable = DepthEnable;
		changed = true;
	}

	auto DepthWriteMask = (int)desc.DepthWriteMask;
	if (reflectEnumeration(type.get_property("DepthWriteMask"), DepthWriteMask))
	{
		desc.DepthWriteMask = (D3D11_DEPTH_WRITE_MASK)DepthWriteMask;
		changed = true;
	}

	auto DepthFunc = (int)desc.DepthFunc;
	if (reflectEnumeration(type.get_property("DepthFunc"), DepthFunc))
	{
		desc.DepthFunc = (D3D11_COMPARISON_FUNC)DepthFunc;
		changed = true;
	}

	auto StencilEnable = (bool)desc.StencilEnable;
	if (reflectBool(type.get_property("StencilEnable"), StencilEnable))
	{
		desc.StencilEnable = StencilEnable;
		changed = true;
	}

	int StencilReadMask = desc.StencilReadMask;
	if (reflectInt(type.get_property("StencilReadMask"), StencilReadMask, 1.0f, 0, 255))
	{
		desc.StencilReadMask = (unsigned char)StencilReadMask;
		changed = true;
	}

	int StencilWriteMask = desc.StencilWriteMask;
	if (reflectInt(type.get_property("StencilWriteMask"), StencilWriteMask, 1.0f, 0, 255))
	{
		desc.StencilWriteMask = (unsigned char)StencilWriteMask;
		changed = true;
	}

	auto FrontFace = desc.FrontFace;
	if (reflectDepthStencilOpDesc("FrontFace-", FrontFace))
	{
		desc.FrontFace = FrontFace;
		changed = true;
	}

	auto BackFace = desc.BackFace;
	if (reflectDepthStencilOpDesc("BackFace-", BackFace))
	{
		desc.BackFace = BackFace;
		changed = true;

	}

	if (changed)
	{
		property.set_value(object, desc);
	}
}

void PropertyPanel::reflectBlendDesc(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	bool changed = false;

	D3D11_BLEND_DESC desc;
	property.get_value(object).convert(desc);

	auto type = rttr::type::get<D3D11_BLEND_DESC>();

	auto AlphaToCoverageEnable = (bool)desc.AlphaToCoverageEnable;
	if (reflectBool(type.get_property("AlphaToCoverageEnable"), AlphaToCoverageEnable))
	{
		desc.AlphaToCoverageEnable = AlphaToCoverageEnable;
		changed = true;
	}

	auto IndependentBlendEnable = (bool)desc.IndependentBlendEnable;
	if (reflectBool(type.get_property("IndependentBlendEnable"), IndependentBlendEnable))
	{
		desc.IndependentBlendEnable = IndependentBlendEnable;
		changed = true;
	}

	auto rendertargetBlendDesc = desc.RenderTarget[0];
	if (reflectRendertargetBlendDesc(rendertargetBlendDesc))
	{
		desc.RenderTarget[0] = rendertargetBlendDesc;
		changed = true;
	}

	if (changed)
	{
		property.set_value(object, desc);
	}
}

void PropertyPanel::reflectButton(const rttr::property& property, std::shared_ptr<Destiny::Object> object)
{
	if (ImGui::Button(property.get_name().data()))
	{
		property.set_value(object, Destiny::Button());
	}
}

void PropertyPanel::reflectMaterial(std::shared_ptr<Destiny::Object> object)
{
	if (!object)
	{
		return;
	}

	auto type = rttr::type::get(*object);
	ImGui::Text((type.get_name().to_string() + ":").c_str());
	for (const auto& property : type.get_properties())
	{
		reflectProperty(property, object);
	}

}

bool PropertyPanel::reflectDepthStencilOpDesc(const std::string& name, D3D11_DEPTH_STENCILOP_DESC& desc)
{
	bool changed = false;

	auto type = rttr::type::get<D3D11_DEPTH_STENCILOP_DESC>();

	auto StencilFailOp = (int)desc.StencilFailOp;
	if (reflectEnumeration(type.get_property("StencilFailOp"), StencilFailOp, name))
	{
		desc.StencilFailOp = (D3D11_STENCIL_OP)StencilFailOp;
		changed = true;
	}
	
	auto StencilDepthFailOp = (int)desc.StencilDepthFailOp;
	if (reflectEnumeration(type.get_property("StencilDepthFailOp"), StencilDepthFailOp, name))
	{
		desc.StencilDepthFailOp = (D3D11_STENCIL_OP)StencilDepthFailOp;
		changed = true;
	}

	auto StencilPassOp = (int)desc.StencilPassOp;
	if (reflectEnumeration(type.get_property("StencilPassOp"), StencilPassOp, name))
	{
		desc.StencilPassOp = (D3D11_STENCIL_OP)StencilPassOp;
		changed = true;
	}

	auto StencilFunc = (int)desc.StencilFunc;
	if (reflectEnumeration(type.get_property("StencilFunc"), StencilFunc, name))
	{
		desc.StencilFunc = (D3D11_COMPARISON_FUNC)StencilFunc;
		changed = true;
	}

	return changed;
}

bool PropertyPanel::reflectRendertargetBlendDesc(D3D11_RENDER_TARGET_BLEND_DESC& desc)
{
	bool changed = false;

	auto type = rttr::type::get<D3D11_RENDER_TARGET_BLEND_DESC>();

	auto BlendEnable = (bool)desc.BlendEnable;
	if (reflectBool(type.get_property("BlendEnable"), BlendEnable))
	{
		desc.BlendEnable = BlendEnable;
		changed = true;
	}

	auto SrcBlend = (int)desc.SrcBlend;
	if (reflectEnumeration(type.get_property("SrcBlend"), SrcBlend))
	{
		desc.SrcBlend = (D3D11_BLEND)SrcBlend;
		changed = true;
	}

	auto DestBlend = (int)desc.DestBlend;
	if (reflectEnumeration(type.get_property("DestBlend"), DestBlend))
	{
		desc.DestBlend = (D3D11_BLEND)DestBlend;
		changed = true;
	}

	auto BlendOp = (int)desc.BlendOp;
	if (reflectEnumeration(type.get_property("BlendOp"), BlendOp))
	{
		desc.BlendOp = (D3D11_BLEND_OP)BlendOp;
		changed = true;
	}

	auto SrcBlendAlpha = (int)desc.SrcBlendAlpha;
	if (reflectEnumeration(type.get_property("SrcBlendAlpha"), SrcBlendAlpha))
	{
		desc.SrcBlendAlpha = (D3D11_BLEND)SrcBlendAlpha;
		changed = true;
	}

	auto DestBlendAlpha = (int)desc.DestBlendAlpha;
	if (reflectEnumeration(type.get_property("DestBlendAlpha"), DestBlendAlpha))
	{
		desc.DestBlendAlpha = (D3D11_BLEND)DestBlendAlpha;
		changed = true;
	}

	auto BlendOpAlpha = (int)desc.BlendOpAlpha;
	if (reflectEnumeration(type.get_property("BlendOpAlpha"), BlendOpAlpha))
	{
		desc.BlendOpAlpha = (D3D11_BLEND_OP)BlendOpAlpha;
		changed = true;
	}

	int RenderTargetWriteMask = desc.RenderTargetWriteMask;
	if (reflectInt(type.get_property("RenderTargetWriteMask"), RenderTargetWriteMask, 1.0f, 0, 255))
	{
		desc.RenderTargetWriteMask = (unsigned char)RenderTargetWriteMask;
		changed = true;
	}

	return changed;
}

bool PropertyPanel::reflectFloat(const rttr::property& property, float& value)
{
	bool changed = false;

	ImGui::PushID(property.get_name().data());
	ImGui::Columns(2);
	ImGui::Text(property.get_name().data());
	ImGui::NextColumn();
	if (ImGui::DragFloat(("##" + property.get_name().to_string()).c_str(), &value, m_dragFloatStep, 0.0f, 0.0f, "%.6f"))
	{
		changed = true;
	}
	ImGui::Columns(1);
	ImGui::PopID();

	return changed;
}

bool PropertyPanel::reflectFloat3(const rttr::property& property, DirectX::XMFLOAT3& value)
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
	if (ImGui::DragFloat("##X", &value.x, m_dragFloatStep, 0.0f, 0.0f, "%.6f"))
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
	if (ImGui::DragFloat("##Y", &value.y, m_dragFloatStep, 0.0f, 0.0f, "%.6f"))
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
	if (ImGui::DragFloat("##Z", &value.z, m_dragFloatStep, 0.0f, 0.0f, "%.6f"))
	{
		changed = true;
	}
	ImGui::PopItemWidth();


	ImGui::PopStyleVar();

	ImGui::Columns(1);
	ImGui::PopID();

	return changed;
}

bool PropertyPanel::reflectEnumeration(const rttr::property& property, int& value, const std::string& name)
{
	bool changed = false;

	auto enumeration = property.get_enumeration();

	std::vector<std::string> items;
	for (const auto& value : enumeration.get_values())
	{
		items.push_back(value.to_string());
	}

	ImGui::Text((name + property.get_name().to_string() + " :").c_str());
	ImGui::SameLine();

	//int itemIndex = property.get_value(object).to_int();
	auto itemIndex = (int)std::distance(items.begin(), std::find(items.begin(), items.end(), enumeration.value_to_name(value).to_string()));
	auto offset = value - itemIndex;
	if (ImGui::Combo(("##" + property.get_name().to_string() + name).c_str(), &itemIndex,
		[](void* data, int idx, const char** out_text)
		{
			auto& vector = *static_cast<std::vector<std::string>*>(data);
			if (idx < 0 || idx >= static_cast<int>(vector.size())) {
				return false;
			}
			*out_text = vector[idx].c_str();
			return true;
		},
		&items, (int)items.size()))
	{
		value = itemIndex + offset;
		changed = true;
	}

	return changed;
}

bool PropertyPanel::reflectBool(const rttr::property& property, bool& value)
{
	bool changed = false;
	ImGui::Text((property.get_name().to_string() + " :").c_str());
	ImGui::SameLine();
	if (ImGui::Checkbox(("##" + property.get_name().to_string()).c_str(), &value))
	{
		changed = true;
	}
	return changed;
}

bool PropertyPanel::reflectInt(const rttr::property& property, int& value, float speed, int min, int max)
{
	bool changed = false;

	ImGui::PushID(property.get_name().data());
	ImGui::Columns(2);
	ImGui::Text(property.get_name().data());
	ImGui::NextColumn();
	if (ImGui::DragInt(("##" + property.get_name().to_string()).c_str(), &value, speed, min, max))
	{
		changed = true;
	}
	ImGui::Columns(1);
	ImGui::PopID();

	return changed;
}

void PropertyPanel::onChoosedNode(void* parameter)
{
	if (!parameter)
	{
		m_choosedNode = nullptr;
		return;
	}
	m_choosedNode = *(std::shared_ptr<Destiny::Node>*) parameter;
}
