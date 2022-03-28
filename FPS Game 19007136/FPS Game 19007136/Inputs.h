#pragma once
#include <Windows.h>
#include <dinput.h>
#include <dxgi.h>
#include <DirectXMath.h>

namespace dx = DirectX;


class Inputs
{
public:
	IDirectInputDevice8* keyboardInputDevice;
	IDirectInputDevice8* mouseInputDevice;

	DIMOUSESTATE mousePreviousState;
	LPDIRECTINPUT8 DirectInput;

	dx::XMVECTOR baseForward = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	dx::XMVECTOR cameraForward = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	dx::XMMATRIX cameraRotationMatrix;
	dx::XMMATRIX groundWorld;

	dx::XMVECTOR moveForward = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	float cameraYaw = 0.0f;
	float camPitch = 0.0f;

	float rotx = 0;
	float rotz = 0;
	float scaleX = 1.0f;
	float scaleY = 1.0f;

	bool shoot = false;
	bool enter = false;
	bool leave = false;

	dx::XMMATRIX Rotationx;
	dx::XMMATRIX Rotationz;

	bool InitDirectInput(HINSTANCE hInstance, HRESULT hresult, HWND hwnd);
	void DetectInput(double time, HWND hwnd);


};

