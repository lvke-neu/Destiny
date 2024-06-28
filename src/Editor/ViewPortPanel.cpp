#include "ViewPortPanel.h"
#include "Imgui/imgui.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/RenderTargetView.h"


void ViewPortPanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

	ImGui::Begin("Viewport");

	onViewPortResize();

	auto rtv = Destiny::Engine::GetInstance()->getGraphicsSystem()->getRTTRenderTargetView();
	if (rtv && rtv->isLoadingSucceed())
	{
		ImGui::Image(*(rtv->getShaderResourceView()), { (float)rtv->getWidth(), (float)rtv->getHeight() });
	}
	
	ImGui::End();

	ImGui::PopStyleVar();
}

void ViewPortPanel::onViewPortResize()
{
	auto viewport = ImGui::GetContentRegionAvail();
	if (m_width != viewport.x || m_height != viewport.y)
	{
		m_width = (unsigned int)viewport.x;
		m_height = (unsigned int)viewport.y;
		Destiny::WindowResizeData windowResizeData;
		windowResizeData.width = m_width;
		windowResizeData.height = m_height;
		Destiny::Engine::GetInstance()->getEventSystem()->dispatchEvent(Destiny::EventType::WindowResize, &windowResizeData);
	}
}

	