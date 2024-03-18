#include "Editor/Application.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR cmdLine, _In_ int showCmd)
{
	Application application;
	application.initialize(hInstance);
	application.run();
	application.uninitialize();

	return 0;
}