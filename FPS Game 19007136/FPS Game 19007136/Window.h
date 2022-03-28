#pragma once
#include <Windows.h>
#include "Game.h"
#include "StartMenu.h"
#include <dxgi.h>
#include <DirectXMath.h>
#include <fstream>
#include <iostream>
#include <string>


enum States
{
	IN_START_MENU = 0,
	IN_GAME = 1,
	IN_END_MENU = 2
};

namespace dx = DirectX;
using namespace std;

class Window
{

public:

	bool InitializeDirect3d11App(HINSTANCE hInstance);
	void CleanUp();
	bool InitScene();
	void DrawScene(double time);
	int messageloop();
	bool InitializeWindow(HINSTANCE hInstance,
		int ShowWnd,
		int width, int height,
		bool windowed);
	void InitDirectInput(HINSTANCE hInstance);
	void ReadMap();

	States states = IN_START_MENU;

	const int Width = 800;
	const int Height = 600;


	bool draw = true;


	static const int width = 20;
	static const int height = 20;

	int index = 0;
	int floorIndex = 0;
	int enemies = 0;
	
	std::pair<int, int> wallPos[width * height * 3];
	std::pair<int, int> floorPos[width * height];
	std::pair<int, int> enemyPos[width * height];
	std::pair<int, int> startPos;

	//std::vector<std::unique_ptr<BillBoard>> billboard;
	//BillBoard billboard;


private:

	IDXGISwapChain* swapchain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* backbuffer;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;
	
	LPCTSTR WndClassName = L"FPS Game";
	HWND hwnd = NULL;
	HRESULT hresult;

	Cube cube;


	int enemiesDead = 0;

	dx::XMFLOAT3 camPosReset = { 30.0f, 3.0f, -30.0f };


	Game game;
	StartMenu startMenu;
	StartMenu endMenu;

	fstream indata;
	char ch;
	int lines = 1;
	int chr = 0;

	bool test = true;
	dx::XMFLOAT3 Pos;


};



