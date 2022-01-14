#pragma once

#include "BillBoard.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <dxgi.h>
#include <d3dcompiler.h>

namespace dx = DirectX;

class Graphics
{
public:


	void CleanUp();
	void CreateShaders(HRESULT hresult, ID3D11Device* dev,
		ID3D11DeviceContext* devcon);



	const int Width = 800;
	const int Height = 600;

	ID3D11Buffer* vertexBuffer, * indexBuffer, * vertexBuffer_, * indexBuffer_;
	UINT stride;
	UINT offset;

private:
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D10Blob* VS_Buffer, * PS_Buffer;
	ID3D11InputLayout* vertLayout;



};

