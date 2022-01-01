#include "Window.h"

int WINAPI WinMain(HINSTANCE hInstance,	//Main windows function
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	Window window;

	window.InitializeWindow(hInstance, nShowCmd, window.Width, window.Height, true);
	window.InitializeDirect3d11App(hInstance);
	window.InitScene();
	window.InitDirectInput(hInstance);
	window.messageloop();

	window.CleanUp();

	return 0;
}
