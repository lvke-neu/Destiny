#pragma once
#include <Windows.h>
#include <memory>

class ViewPortPanel;
class ScenePanel;
class PropertyPanel;
class AssetPanel;
class DebugDeferredRenderPanel;
class DebugShadowMapPanel;
class GraphicsStatPanel;
class Application
{
public:
	Application();
public:
	void initialize(HINSTANCE hInstance);
	void run();
	void uninitialize();
private:
	void initImGui();
	void drawDock();
	void newScene();
	void saveScene();
	void openScene();
	void clearScene();

	void fileMenu();
	void viewMenu();
	void settingMenu();
private:
	HWND										m_hwnd;
	HINSTANCE									m_hInstance;
public:
	std::shared_ptr<ViewPortPanel>					m_viewPortPanel;
	std::shared_ptr<ScenePanel>						m_scenePanel;
	std::shared_ptr<PropertyPanel>					m_propertyPanel;
	std::shared_ptr<AssetPanel>						m_assetPanel;
	std::shared_ptr<DebugDeferredRenderPanel>		m_debugDeferredRenderPanel;
	std::shared_ptr<DebugShadowMapPanel>			m_debugShadowMapPanel;
	std::shared_ptr<GraphicsStatPanel>				m_graphicsStatPanel;
};

	