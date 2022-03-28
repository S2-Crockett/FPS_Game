#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXTex.h>

namespace dx = DirectX;

class BillBoard
{
public:

	void DrawEnemy(ID3D11DeviceContext* deviceContext, float x, float y, float z, dx::XMMATRIX cameraView, dx::XMMATRIX cameraProjection);

	void UpdateEnemy(double time, dx::XMFLOAT3 camPos);

	void CreateBuffer(HRESULT hresult, ID3D11Device* dev, const wchar_t* file);

	ID3D11ShaderResourceView* cubeTextureView;
	ID3D11Resource* textureReasource;

	ID3D11Buffer* cbPerObjectBuffer;

	dx::XMMATRIX Rotation;
	dx::XMMATRIX Scale;
	dx::XMMATRIX Translation;

	dx::XMMATRIX WVP;
	dx::XMMATRIX cube;

	dx::XMFLOAT3 pos;
	dx::XMFLOAT3X3 rotation;
	dx::XMFLOAT3 scale = { 2.0f, 3.0f, 2.0f };

	dx::XMVECTOR position;

	float rot = 0.0f;

	bool active = true;
};

