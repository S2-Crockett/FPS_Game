#pragma once
#include <Windows.h>
#include "Game.h"
#include <dxgi.h>
#include <DirectXMath.h>
#include <fstream>
#include <iostream>
#include <string>


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
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;
	ID3D11RenderTargetView* backbuffer;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;
	
	LPCTSTR WndClassName = L"FPS Game";
	HWND hwnd = NULL;
	HRESULT hresult;

	Cube cube;


	int enemiesDead = 0;


	Game game;


	fstream indata;
	char ch;
	int lines = 1.0f;
	int chr = 0.0f;

	bool test = true;
	dx::XMFLOAT3 Pos;


};



