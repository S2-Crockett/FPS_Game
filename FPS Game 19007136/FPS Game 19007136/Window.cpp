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
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WndClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

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
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = Width;
	bufferDesc.Height = Height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	//Create our SwapChain
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &swapchain, &dev, NULL, &devcon);

	//Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	//Create our Render Target
	dev->CreateRenderTargetView(BackBuffer, NULL, &backbuffer);
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
	dev->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	dev->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	//Set our Render Target
	devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);

	return true;
}

void Window::CleanUp()
{
	//Release the COM Objects we created
	swapchain->Release();
	dev->Release();
	devcon->Release();
	backbuffer->Release();

	GFX.CleanUp();

	depthStencilView->Release();
	depthStencilBuffer->Release();
}



bool Window::InitScene()
{
	GFX.CreateShaders(hresult, dev, devcon);
	cube.CreateBuffer(hresult, dev);
	cube.CreateTexture(hresult, dev, L"Image.jpg");
	return true;
}

void Window::InitDirectInput(HINSTANCE hInstance)
{
	GFX.camera.InitDirectInput(hInstance, hresult, hwnd);
}


void Window::Collision(std::vector<bool> &collidedFront, bool &collided)
{
	if (std::all_of(std::begin(collidedFront), std::end(collidedFront), [](bool i) {return !i; }))
	{
		collided = false;
	}
	for (int j = 0; j < collidedFront.size(); j++)
	{
		if (collidedFront.at(j) == true)
		{
			collidedFront.at(j) = false;
		}
	}
}


void Window::UpdateScene()
{
	Collision(collidedFront, GFX.camera.collidedFront);
	Collision(collidedBack, GFX.camera.collidedBack);
	Collision(collidedLeft, GFX.camera.collidedLeft);
	Collision(collidedRight, GFX.camera.collidedRight);
	for (int i = 0; i < index; i++)
	{
		if (GFX.camera.Intersecting(GFX.camera.cube[i].pos, GFX.camera.cube[i].scale))
		{
			GFX.camera.Resolve(GFX.camera.cube[i].pos, GFX.camera.cube[i].scale, collidedFront, collidedBack, collidedLeft, collidedRight, i);
		}
	}
}

void Window::DrawScene(double time)
{
	//Clear our backbuffer to the updated color
	const float bgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	devcon->ClearRenderTargetView(backbuffer, bgColor);
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//GFX.camera.SetMatrix(devcon);
	devcon->IASetIndexBuffer(GFX.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetVertexBuffers(0, 1, &GFX.vertexBuffer, &GFX.stride, &GFX.offset);
	GFX.camera.Shoot(devcon, time, hresult, dev);
	ReadMap();
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
		else if (ch == '~')
		{
			floorPos[floorIndex] = make_pair(chr, lines);
			floorIndex++;
			chr++; 
		}
		else if (ch == 'e')
		{
			enemyPos[enemies] = make_pair(chr, lines);
			enemies++;
			floorPos[floorIndex] = make_pair(chr, lines);
			floorIndex++;
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
		devcon->IASetIndexBuffer(GFX.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		devcon->IASetVertexBuffers(0, 1, &GFX.vertexBuffer, &GFX.stride, &GFX.offset);
		GFX.camera.DrawCube(devcon, wallPos[i].first * 4, 0, -wallPos[i].second * 4, i);
	}
	for (int f = 0; f < floorIndex; f++)
	{
		devcon->IASetIndexBuffer(GFX.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		devcon->IASetVertexBuffers(0, 1, &GFX.vertexBuffer, &GFX.stride, &GFX.offset);
		GFX.camera.DrawFloorCube(devcon, floorPos[f].first * 4, -5.0f, -floorPos[f].second * 4, f);
	}
	devcon->IASetIndexBuffer(GFX.indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetVertexBuffers(0, 1, &GFX.vertexBuffer_, &GFX.stride, &GFX.offset);

	for (int e = 0; e < enemies; e++)
	{
		if (billboard[e].active)
		{
			billboard[e].CreateBuffer(hresult, dev, L"Image1.jpg");
			billboard[e].DrawCube(devcon, enemyPos[e].first * 4, 3, -enemyPos[e].second * 4, GFX.camera.camView, GFX.camera.camProjection);
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

			timer.RunTimer();
			DrawScene(timer.frameTime);
			GFX.camera.DetectInput(timer.frameTime, hwnd);
			UpdateScene();

			for (auto& bullets : GFX.camera.bullet)
			{
				for (int e = 0; e < enemies; e++)
				{
					if (bullets->pos.x < billboard[e].pos.x + 3 &&
						bullets->pos.x > billboard[e].pos.x - 3 &&
						bullets->pos.z < billboard[e].pos.z + 3 &&
						bullets->pos.z > billboard[e].pos.z - 3 &&
						billboard[e].active)
					{
						bullets->active = false;
						billboard[e].active = false;
					}
				}


			}

			for (int i = 0; i < index; i++)
			{
				GFX.camera.UpdateCamera(timer.frameTime, GFX.camera.cube[i].pos);
			}
			for (int e = 0; e < enemies; e++)
			{
				billboard[e].UpdateBillboard(timer.frameTime, GFX.camera.camPos);
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



