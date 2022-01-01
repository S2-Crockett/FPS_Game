#pragma once
#include <Windows.h>
#include <dinput.h>
#include <dxgi.h>
#include <DirectXMath.h>

namespace dx = DirectX;


class Inputs
{
public:
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;

	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 DirectInput;

	dx::XMVECTOR DefaultForward = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	dx::XMVECTOR DefaultRight = dx::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	dx::XMVECTOR camForward = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	dx::XMVECTOR camRight = dx::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	dx::XMMATRIX camRotationMatrix;
	dx::XMMATRIX groundWorld;

	dx::XMVECTOR moveLeftRight = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	dx::XMVECTOR moveBackForward = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	float camYaw = 0.0f;
	float camPitch = 0.0f;

	float rotx = 0;
	float rotz = 0;
	float scaleX = 1.0f;
	float scaleY = 1.0f;

	bool shoot = false;

	dx::XMMATRIX Rotationx;
	dx::XMMATRIX Rotationz;

	bool InitDirectInput(HINSTANCE hInstance, HRESULT hresult, HWND hwnd);
	void DetectInput(double time, HWND hwnd);


};

