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
#if 0
	if (m_open)
    // ...
    // ...
#endif
}

void DebugDeferredRenderPanel::setOpen(bool open)
{
	m_open = open;
}