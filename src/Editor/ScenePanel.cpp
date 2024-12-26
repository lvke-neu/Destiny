#include "ScenePanel.h"
#include "Application.h"
#include "StatPanel.h"
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
	
	if (ImGui::TreeNodeEx(node->get_name().c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected))
	{
		if (m_application && m_application->m_statPanel)
		{
			m_application->m_statPanel->setChoosedNode(node);
			send(ChoosedNode);
		}

		for (const auto& child : node->getChilds())
		{
			traversal(child);
		}

		ImGui::TreePop();
	}
}

ScenePanel::ScenePanel(Application* application) :
	m_application(application)
{

}

void ScenePanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

	ImGui::Begin("SceneNode");

	traversal(Destiny::Engine::GetInstance()->getSceneManager()->getScene());

	ImGui::End();

	ImGui::PopStyleVar();
}
	