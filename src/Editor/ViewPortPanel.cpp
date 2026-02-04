#include "ViewPortPanel.h"
#include "Imgui/imgui.h"
#include "ImGui/ImGuizmo.h"
#include "Engine/Engine.h"
#include "Engine/EventSystem.h"
#include "Engine/FileSystem.h"
#include "Engine/UnSerializer.h"
#include "Engine/Blob.h"
#include "Engine/BlobLoaderManager.h"
#include "Engine/BlobLoader.h"
#include "Engine/BlobHolder.h"
#include "Graphics/GraphicsSystem.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/RenderTargetView.h"
#include "Graphics/BindRenderTargets.h"
#include "Graphics/VisualScene.h"
#include "Graphics/CameraComponent.h"
#include "Scene/Model/ModelComponent.h"
#include "Scene/SceneManager.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

ViewPortPanel::ViewPortPanel() :
	m_choosedNode(nullptr),
	m_gzimoType(0),
	m_gzimoMode(0)
{

}

void ViewPortPanel::update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Viewport"); 
	
    auto rs = std::static_pointer_cast<Destiny::RenderSystem>(Destiny::Engine::GetInstance()->getGraphicsSystem());
    void* tex = rs->getViewportTextureID();
    
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    m_width = (unsigned int)viewportSize.x;
    m_height = (unsigned int)viewportSize.y;

    if (tex)
    {
        ImGui::Image((ImTextureID)tex, viewportSize);
    }
    
    // Original logic placeholders
    // if (ImGui::IsWindowFocused()) ...
    
	ImGui::End();
	ImGui::PopStyleVar();
}

// ...

void ViewPortPanel::processGzimo()
{
#if 0
	if (!m_choosedNode)
    // ...
#endif
}

void ViewPortPanel::onChoosedNode(void* parameter)
{
	if (!parameter)
	{
		m_choosedNode = nullptr;
		return;
	}
	m_choosedNode = *(std::shared_ptr<Destiny::Node>*) parameter;
}

void ViewPortPanel::setGzimoType(int gzimoType)
{
	m_gzimoType = gzimoType;
}

	