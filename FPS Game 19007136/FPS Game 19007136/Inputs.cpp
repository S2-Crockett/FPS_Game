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
        &DIKeyboard,
        NULL);

    hresult = DirectInput->CreateDevice(GUID_SysMouse,
        &DIMouse,
        NULL);

    hresult = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
    hresult = DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

    hresult = DIMouse->SetDataFormat(&c_dfDIMouse);
    hresult = DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

    return true;
}


void Inputs::DetectInput(double time, HWND hwnd)
{
    DIMOUSESTATE mouseCurrState;

    BYTE keyboardState[256];

    DIKeyboard->Acquire();
    DIMouse->Acquire();

    DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

    DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

    if (keyboardState[DIK_ESCAPE] & 0x80)
        PostMessage(hwnd, WM_DESTROY, 0, 0);

    dx::XMVECTOR speed = dx::XMVectorSet(15.0f * time, 0.0f, 15.0f * time, 0.0f);

    if (keyboardState[DIK_A] & 0x80)
    {
        moveLeftRight = dx::XMVectorSubtract(moveLeftRight, speed);
    }
    if (keyboardState[DIK_D] & 0x80)
    {
        moveLeftRight = dx::XMVectorAdd(moveLeftRight, speed);
    }
    if (keyboardState[DIK_W] & 0x80)
    {
        moveBackForward = dx::XMVectorAdd(moveBackForward, speed);
    }
    if (keyboardState[DIK_S] & 0x80)
    {
        moveBackForward = dx::XMVectorSubtract(moveBackForward, speed);
    }
    if (keyboardState[DIK_F] & 0x80)
    {
        shoot = true;
    }
	else
	{
		shoot = false;
	}
    if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
    {
        camYaw += mouseLastState.lX * 0.001f;

        camPitch += mouseCurrState.lY * 0.001f;

        mouseLastState = mouseCurrState;
    }

    return;
}
