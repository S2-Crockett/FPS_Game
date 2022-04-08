#include <algorithm>
#include "Window.h"
#pragma comment(lib, "d3d11.lib")



LRESULT CALLBACK WndProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

bool Window::InitializeWindow(HINSTANCE hInstance,
	int ShowWnd,
	int width, int height,
	bool windowed)
{
	WNDCLASSEX windowClass;

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = NULL;
	windowClass.cbWndExtra = NULL;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = WndClassName;
	windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&windowClass);

	hwnd = CreateWindowEx(
		NULL,
		WndClassName,
		L"FPS Game",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hwnd)
	{
		MessageBox(NULL, L"Error creating window",
			L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(hwnd, ShowWnd);
	UpdateWindow(hwnd);

	return true;
}
bool Window::InitializeDirect3d11App(HINSTANCE hInstance)
{
	//Describe our SwapChain Buffer
	DXGI_MODE_DESC swapchainBufferDesc;

	ZeroMemory(&swapchainBufferDesc, sizeof(DXGI_MODE_DESC));

	swapchainBufferDesc.Width = Width;
	swapchainBufferDesc.Height = Height;
	swapchainBufferDesc.RefreshRate.Numerator = 60;
	swapchainBufferDesc.RefreshRate.Denominator = 1;
	swapchainBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainBufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchainBufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = swapchainBufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	//Create our SwapChain
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &swapchain, &device, NULL, &deviceContext);

	//Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	//Create our Render Target
	if (BackBuffer != 0)
	{
		device->CreateRenderTargetView(BackBuffer, NULL, &backbuffer);
	}
	BackBuffer->Release();

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = Width;
	depthStencilDesc.Height = Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//Create the Depth/Stencil View
	device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	if (depthStencilBuffer != 0)
	{
		device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
	}

	//Set our Render Target
	deviceContext->OMSetRenderTargets(1, &backbuffer, depthStencilView);

	return true;
}
void Window::CleanUp()
{
	//Release the COM Objects we created
	swapchain->Release();
	device->Release();
	deviceContext->Release();
	backbuffer->Release();

	game.GFX.CleanUp();

	depthStencilView->Release();
	depthStencilBuffer->Release();
}

bool Window::InitScene()
{
	game.GFX.CreateShaders(hresult, device, deviceContext);
	game.CreateBuffer(hresult, device, deviceContext, hwnd);
	cube.CreateBuffer(hresult, device);
	cube.CreateTexture(hresult, device, L"Images/Image.jpg");
	return true;
}

void Window::InitDirectInput(HINSTANCE hInstance)
{
	game.camera.InitDirectInput(hInstance, hresult, hwnd);
	startMenu.input.InitDirectInput(hInstance, hresult, hwnd);
	endMenu.input.InitDirectInput(hInstance, hresult, hwnd);
}

void Window::DrawScene(double time)
{
	//Clear our backbuffer to the updated color
	const float bgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	deviceContext->ClearRenderTargetView(backbuffer, bgColor);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	deviceContext->IASetIndexBuffer(game.GFX.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetVertexBuffers(0, 1, &game.GFX.vertexBuffer, &game.GFX.stride, &game.GFX.offset);

	if (states == IN_GAME)
	{
		game.DrawScene();
		ReadMap();
	}
	else if (states == IN_START_MENU)
	{
		startMenu.CreateBuffer(hresult, device, L"Images/StartMenuImage.jpg");
		game.camera.cameraPos = camPosReset;
		startMenu.DrawCube(deviceContext, 20,2,-20, game.camera.cameraView, game.camera.cameraProjection);
		startMenu.UpdateBillboard(game.timer.frameTime, game.camera.cameraPos);
	}
	else if (states == IN_END_MENU)
	{
		endMenu.CreateBuffer(hresult, device, L"Images/ExitMenuImage.jpg");
		game.camera.cameraPos = camPosReset;

		game.camera.cameraPosition = dx::XMVectorSet(game.camera.cameraPos.x, game.camera.cameraPos.y, game.camera.cameraPos.z, 0.0f);
		game.camera.cameraTarget = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		game.camera.cameraUpDir = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		//Set the View matrix
		game.camera.cameraView = dx::XMMatrixLookAtLH(game.camera.cameraPosition, game.camera.cameraTarget, game.camera.cameraUpDir);

		endMenu.DrawCube(deviceContext, 20, 2, -20, game.camera.cameraView, game.camera.cameraProjection);
		endMenu.UpdateBillboard(game.timer.frameTime, game.camera.cameraPos);
	}

	//Present the backbuffer to the screen
	swapchain->Present(0, 0);
}

void Window::ReadMap()
{
	indata.open("Map.txt");
	indata.get(ch);

	while (indata)
	{
		if (ch == '\n')
		{
			lines++;
			chr = 0;
		}
		else if (ch == '#')
		{
			wallPos[index] = make_pair(chr, lines);
			index++;
			chr++;
		}
		else if (ch == 'e')
		{
			enemyPos[enemies] = make_pair(chr, lines);
			enemies++;
			chr++;
		}
		else if (ch == 's')
		{
			startPos = make_pair(chr, lines);
			chr++;
		}
		else
		{
			chr++;
		}
		indata.get(ch);
	}
	for (int i = 0; i < index; i++)
	{		
		deviceContext->IASetIndexBuffer(game.GFX.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetVertexBuffers(0, 1, &game.GFX.vertexBuffer, &game.GFX.stride, &game.GFX.offset);
		game.camera.DrawCube(deviceContext, (float)wallPos[i].first * 4, 0, (float)-wallPos[i].second * 4, i);
	}

	if (test)
	{
		Pos = dx::XMFLOAT3((float)startPos.first * 4, 3.0f, (float)-startPos.second * 4);
		dx::XMVECTOR PosVector;
		PosVector = dx::XMLoadFloat3(&Pos);
		game.camera.cameraPosition = PosVector;
		test = false;
	}

	game.camera.DrawFloorCube(deviceContext, game.camera.floorCube.scale.x + 2, -5, -game.camera.floorCube.scale.z - 18, 0);

	for (int e = 0; e < enemies; e++)
	{
		game.billboard_.emplace_back(new BillBoard);

		if (game.billboard_.at(e).get()->active)
		{		
			game.billboard_.at(e).get()->CreateBuffer(hresult, device, L"Images/Image1.jpg");
			game.billboard_.at(e).get()->DrawEnemy(deviceContext, (float)enemyPos[e].first * 4, 3, (float)-enemyPos[e].second * 4, game.camera.cameraView, game.camera.cameraProjection);
		}
	}
}

int Window::messageloop() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		BOOL PeekMessageL(
			LPMSG lpMsg,
			HWND hWnd,
			UINT wMsgFilterMin,
			UINT wMsgFilterMax,
			UINT wRemoveMsg
		);

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

			game.Update(enemies, index, states);
			DrawScene(game.timer.frameTime);
			switch (states)
			{
			case IN_START_MENU:
			{
				startMenu.DetectInput(game.timer.frameTime, hwnd);
				if (startMenu.input.enter)
				{
					states = IN_GAME;
				}
				break;
			}
			case IN_GAME:
			{
				if (enemies != 0)
				{
					if (game.enemiesDead == enemies)
					{
						states = IN_END_MENU;
					}
				}
				break;
			}
			case IN_END_MENU:
			{
				endMenu.DetectInput(game.timer.frameTime, hwnd);
				if (endMenu.input.leave)
				{
					DestroyWindow(hwnd);
				}
				break;
			}
			}

		}
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			DestroyWindow(hwnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}



