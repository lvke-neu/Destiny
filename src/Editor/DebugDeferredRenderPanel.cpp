#include "DebugDeferredRenderPanel.h"
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

DebugDeferredRenderPanel::DebugDeferredRenderPanel() :
	m_open(false)
{

}

void DebugDeferredRenderPanel::update()
{
	if (m_open)
	{
		const float width = 500.0f;
		const float height = 200.0f;

		ImGui::Begin("DebugDeferredRenderPanel");

		auto deferredOpaquePipeline = std::static_pointer_cast<Destiny::DeferredOpaquePipeline>(std::static_pointer_cast<Destiny::RenderSystem>(Destiny::Engine::GetInstance()->getGraphicsSystem())->getDeferredOpaquePipeline());
		std::shared_ptr<Destiny::RenderTargetView> rtv = nullptr;

		ImGui::Text("Albedo");
		rtv = deferredOpaquePipeline->m_bindRenderTargets->getRenderTargetViews(0);
		if (rtv && rtv->isLoadingSucceed())
		{
			ImGui::Image(*(rtv->getShaderResourceView()), { width, height });
		}

		ImGui::Text("Normal");
		rtv = deferredOpaquePipeline->m_bindRenderTargets->getRenderTargetViews(1);
		if (rtv && rtv->isLoadingSucceed())
		{
			ImGui::Image(*(rtv->getShaderResourceView()), { width, height });
		}

		ImGui::Text("MRA");
		rtv = deferredOpaquePipeline->m_bindRenderTargets->getRenderTargetViews(2);
		if (rtv && rtv->isLoadingSucceed())
		{
			ImGui::Image(*(rtv->getShaderResourceView()), { width, height });
		}

		ImGui::Text("positionW");
		rtv = deferredOpaquePipeline->m_bindRenderTargets->getRenderTargetViews(3);
		if (rtv && rtv->isLoadingSucceed())
		{
			ImGui::Image(*(rtv->getShaderResourceView()), { width, height });
		}

		ImGui::Text("normalW");
		rtv = deferredOpaquePipeline->m_bindRenderTargets->getRenderTargetViews(4);
		if (rtv && rtv->isLoadingSucceed())
		{
			ImGui::Image(*(rtv->getShaderResourceView()), { width, height });
		}

		ImGui::Text("texcoord");
		rtv = deferredOpaquePipeline->m_bindRenderTargets->getRenderTargetViews(5);
		if (rtv && rtv->isLoadingSucceed())
		{
			ImGui::Image(*(rtv->getShaderResourceView()), { width, height });
		}

		ImGui::End();
	}
}

void DebugDeferredRenderPanel::setOpen(bool open)
{
	m_open = open;
}