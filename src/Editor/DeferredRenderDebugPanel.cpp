#include "DeferredRenderDebugPanel.h"
#include "Imgui/imgui.h"
#include "ImGui/ImGuizmo.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/RenderTargetView.h"
#include "Graphics/BindRenderTargets.h"
#include "Graphics/VisualScene.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/DeferredOpaquePipeline.h"
#include "Scene/SceneManager.h"

DeferredRenderDebugPanel::DeferredRenderDebugPanel()
{

}

void DeferredRenderDebugPanel::update()
{
	auto deferredOpaquePipeline = std::static_pointer_cast<Destiny::DeferredOpaquePipeline>(std::static_pointer_cast<Destiny::RenderSystem>(Destiny::Engine::GetInstance()->getGraphicsSystem())->getDeferredOpaquePipeline());

	std::shared_ptr<Destiny::RenderTargetView> rtv = nullptr;

	ImGui::Begin("Albedo");
	rtv = deferredOpaquePipeline->m_bindRenderTargets->getRenderTargetViews(0);
	if (rtv && rtv->isLoadingSucceed())
	{
		ImGui::Image(*(rtv->getShaderResourceView()), { (float)rtv->getWidth(), (float)rtv->getHeight() });
	}
	ImGui::End();

	ImGui::Begin("Normal");
	rtv = deferredOpaquePipeline->m_bindRenderTargets->getRenderTargetViews(1);
	if (rtv && rtv->isLoadingSucceed())
	{
		ImGui::Image(*(rtv->getShaderResourceView()), { (float)rtv->getWidth(), (float)rtv->getHeight() });
	}
	ImGui::End();

	ImGui::Begin("MRA");
	rtv = deferredOpaquePipeline->m_bindRenderTargets->getRenderTargetViews(2);
	if (rtv && rtv->isLoadingSucceed())
	{
		ImGui::Image(*(rtv->getShaderResourceView()), { (float)rtv->getWidth(), (float)rtv->getHeight() });
	}
	ImGui::End();
}
