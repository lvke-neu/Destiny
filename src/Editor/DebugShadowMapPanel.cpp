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
#if 0
	if (m_open)
    // ...
    // ...
#endif
}

void DebugShadowMapPanel::setOpen(bool open)
{
	m_open = open;
}