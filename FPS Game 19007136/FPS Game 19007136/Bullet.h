#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <iostream>
#include <memory>
#include "Cube.h"
#include <iomanip>

namespace dx = DirectX;

class Bullet
{
public:
	Bullet();
	~Bullet() = default;

	void CreateBullet(HRESULT hresult, ID3D11Device* dev, const wchar_t* file, ID3D11DeviceContext* devcon, dx::XMFLOAT3 pos,
					  dx::XMMATRIX camView, dx::XMMATRIX camProjection);

	static const int width = 20;
	static const int height = 20;
	Cube bullet[width * height];

	bool active = false;
	bool set = false;
};

