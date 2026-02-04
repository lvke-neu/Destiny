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
// #include <d3d11.h>

PropertyPanel::PropertyPanel() :
	m_choosedNode(nullptr),
	m_dragFloatStep(1.0f)
{
}

void PropertyPanel::update()
{
#if 0
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
#endif
}

#if 0
void PropertyPanel::setDragFloatStep(float dragFloatStep)
{
	m_dragFloatStep = dragFloatStep;
}

void PropertyPanel::popup(std::shared_ptr<Destiny::Component> component)
{
    // ...
}

void PropertyPanel::reflect(std::shared_ptr<Destiny::Object> object)
{
    // ...
}

// ... all other methods
void PropertyPanel::onChoosedNode(void* parameter)
{
	if (!parameter)
	{
		m_choosedNode = nullptr;
		return;
	}
	m_choosedNode = *(std::shared_ptr<Destiny::Node>*) parameter;
}
#else
void PropertyPanel::onChoosedNode(void* parameter) {}
// float PropertyPanel::getDragFloatStep() { return m_dragFloatStep; }
void PropertyPanel::setDragFloatStep(float dragFloatStep) { m_dragFloatStep = dragFloatStep; }
#endif
