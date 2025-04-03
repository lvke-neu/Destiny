#include "DebugShadowMapPanel.h"
#include "Imgui/imgui.h"
#include "ImGui/ImGuizmo.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/RenderTargetView.h"
#include "Graphics/BindRenderTargets.h"
#include "Graphics/DepthStencilView.h"
#include "Graphics/VisualScene.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/DeferredOpaquePipeline.h"
#include "Graphics/ShadowMapPipeline.h"
#include "Scene/SceneManager.h"

DebugShadowMapPanel::DebugShadowMapPanel() :
	m_open(false)
{

}

void DebugShadowMapPanel::update()
{
	if (m_open)
	{
		const float width = 500.0f;
		const float height = 200.0f;

		ImGui::Begin("DebugShadowMapPanel");

		auto shadowMapPipeline = std::static_pointer_cast<Destiny::ShadowMapPipeline>(std::static_pointer_cast<Destiny::RenderSystem>(Destiny::Engine::GetInstance()->getGraphicsSystem())->getShadowMapPipeline());
		std::shared_ptr<Destiny::DepthStencilView> dsv = nullptr;

		dsv = shadowMapPipeline->m_bindRenderTargets->getDepthStencilViews(0);
		if (dsv && dsv->isLoadingSucceed())
		{
			ImGui::Image(*(dsv->getShaderResourceView()), { width, height });
		}

		ImGui::End();
	}
}

void DebugShadowMapPanel::setOpen(bool open)
{
	m_open = open;
}