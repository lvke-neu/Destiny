#include "StatPanel.h"
#include "Imgui/imgui.h"
#include "Engine/Engine.h"
#include "Engine/Node.h"
#include "Engine/Component.h"
#include "Graphics/VisualComponent.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderPass.h"
#include "Graphics/GraphicsSystem.h"

StatPanel::StatPanel() :
	m_choosedNode(nullptr)
{
}

void StatPanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Stat");

	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("DrawCallCount:%ld", Destiny::Engine::GetInstance()->getGraphicsSystem()->getGraphicsStat().DrawCallCount);
	
	ImGui::Separator();
	
	if (m_choosedNode)
	{
		ImGui::Text("nodeInfo:%s\n%s", m_choosedNode->get_name().c_str(), m_choosedNode->get_transform().toString().c_str());
		ImGui::Separator();
		for (const auto& component : m_choosedNode->getComponents())
		{
			if (component)
			{
				ImGui::Text("%s", component->get_class_name().c_str());
				if (std::dynamic_pointer_cast<Destiny::VisualComponent>(component))
				{
					auto visual = std::dynamic_pointer_cast<Destiny::VisualComponent>(component)->getVisual();
					if (visual && visual->getRenderPass())
					{
						auto renderer = visual->getRenderPass()->getRenderer();
						if (renderer)
						{
							ImGui::Text("Renderer:%s", renderer->getPath().c_str());
						}
					}
					
				}
			}
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void StatPanel::setChoosedNode(std::shared_ptr<Destiny::Node> choosedNode)
{
	m_choosedNode = choosedNode;
}