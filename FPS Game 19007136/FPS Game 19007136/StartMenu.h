#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include "Inputs.h"

namespace dx = DirectX;


class StartMenu
{
	public:

		Inputs input;

		void DrawCube(ID3D11DeviceContext* devcon, float x, float y, float z, dx::XMMATRIX camView, dx::XMMATRIX camProjection);

		void UpdateBillboard(double time, dx::XMFLOAT3 camPos);

		void CreateBuffer(HRESULT hresult, ID3D11Device* dev, const wchar_t* file);

		void InitDirectInput(HINSTANCE hInstance, HRESULT hresult, HWND hwnd);
		void DetectInput(double time, HWND hwnd);

		ID3D11ShaderResourceView* CubesTexture;
		ID3D11Resource* m_texture;

		ID3D11Buffer* cbPerObjectBuffer;

		dx::XMMATRIX Rotation;
		dx::XMMATRIX Scale;
		dx::XMMATRIX Translation;

		dx::XMMATRIX WVP;
		dx::XMMATRIX cube1World;

		dx::XMFLOAT3 pos;
		dx::XMFLOAT3X3 rotation;
		dx::XMFLOAT3 scale = { 5.0f, 5.0f, 5.0f };

		dx::XMVECTOR position;

		float rot = 0.0f;

		bool active = true;
};

