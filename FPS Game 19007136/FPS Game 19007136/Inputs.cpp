#include "Inputs.h"

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

bool Inputs::InitDirectInput(HINSTANCE hInstance, HRESULT hresult, HWND hwnd)
{
    hresult = DirectInput8Create(hInstance,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&DirectInput,
        NULL);

    hresult = DirectInput->CreateDevice(GUID_SysKeyboard,
        &keyboardInputDevice,
        NULL);

    hresult = DirectInput->CreateDevice(GUID_SysMouse,
        &mouseInputDevice,
        NULL);

    hresult = keyboardInputDevice->SetDataFormat(&c_dfDIKeyboard);
    hresult = keyboardInputDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

    hresult = mouseInputDevice->SetDataFormat(&c_dfDIMouse);
    hresult = mouseInputDevice->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

    return true;
}


void Inputs::DetectInput(double time, HWND hwnd)
{
    DIMOUSESTATE mouseCurrState;

    BYTE keyboardState[256];

    keyboardInputDevice->Acquire();
    mouseInputDevice->Acquire();

    mouseInputDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

    keyboardInputDevice->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

    if (keyboardState[DIK_ESCAPE] & 0x80)
        PostMessage(hwnd, WM_DESTROY, 0, 0);

    dx::XMVECTOR speed = dx::XMVectorSet(15.0f * time, 0.0f, 15.0f * time, 0.0f);

    if (keyboardState[DIK_W] & 0x80)
    {
        moveForward = dx::XMVectorAdd(moveForward, speed);
    }
    if (keyboardState[DIK_F] & 0x80)
    {
        shoot = true;
    }
	else
	{
		shoot = false;
	}
    if ((mouseCurrState.lX != mousePreviousState.lX) || (mouseCurrState.lY != mousePreviousState.lY))
    {
        cameraYaw += mousePreviousState.lX * 0.001f;

        camPitch += mouseCurrState.lY * 0.001f;

        mousePreviousState = mouseCurrState;
    }
    if (keyboardState[DIK_SPACE] & 0x80)
    {
        enter = true;
    }
    if (keyboardState[DIK_ESCAPE] & 0x80)
    {
        leave = true;
    }

    return;
}
