#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <iostream>
#include <memory>
#include "Cube.h"
#include "Inputs.h"
#include <iomanip>

namespace dx = DirectX;

class Bullet
{
public:
	Bullet();
	~Bullet() = default;

	void CreateBullet(HRESULT hresult, ID3D11Device* dev, const wchar_t* file, ID3D11DeviceContext* devcon, dx::XMFLOAT3 pos,
		dx::XMMATRIX camView, dx::XMMATRIX camProjection);

	void Shoot(ID3D11DeviceContext* devcon, double timer, HRESULT hresult, ID3D11Device* dev, Inputs input, dx::XMMATRIX camView, dx::XMMATRIX camProjection, dx::XMFLOAT3 rotation, dx::XMFLOAT3 camPos, dx::XMVECTOR camTarget);
	void DrawBullet(ID3D11DeviceContext* devcon);

	static const int width = 20;
	static const int height = 20;
	Cube bullet;

	bool active = false;
	bool set = false;
	dx::XMFLOAT3 result;

	dx::XMFLOAT3 bulletPos;
	bool shot = false;

	dx::XMMATRIX camView;
	dx::XMMATRIX camProjection;

};

