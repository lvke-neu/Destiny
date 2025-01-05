#pragma once
#include <Windows.h>
#include <memory>

class ViewPortPanel;
class ScenePanel;
class PropertyPanel;
class AssetPanel;
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
private:
	HWND								m_hwnd;
	HINSTANCE							m_hInstance;
public:
	std::shared_ptr<ViewPortPanel>		m_viewPortPanel;
	std::shared_ptr<ScenePanel>			m_scenePanel;
	std::shared_ptr<PropertyPanel>		m_propertyPanel;
	std::shared_ptr<AssetPanel>			m_assetPanel;
};

	