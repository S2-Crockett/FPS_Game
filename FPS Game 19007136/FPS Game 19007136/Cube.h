#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <iostream>
#include <memory>
#include <iomanip>

namespace dx = DirectX;

class Cube
{
public:
	Cube();
	~Cube() = default;

	void DrawCube(ID3D11DeviceContext* deviceContext, float x, float y, float z,
				  dx::XMMATRIX cameraView, dx::XMMATRIX cameraProjection);
	void CreateBuffer(HRESULT hresult, ID3D11Device* device);

	void CreateTexture(HRESULT hresult, ID3D11Device* device, const wchar_t* file);

	ID3D11ShaderResourceView* CubesTexture;
	ID3D11Resource* textureResource;

	ID3D11Buffer* constantBuffer;

	dx::XMMATRIX Rotation;
	dx::XMMATRIX Scale;
	dx::XMMATRIX Translation;

	dx::XMMATRIX WVP;
	dx::XMMATRIX cube;

	dx::XMFLOAT3 pos;
	dx::XMFLOAT3 scale = {2.0f, 3.0f, 2.0f};

	dx::XMVECTOR position;

	float rotation = 0.0f;
	bool set = false;
	bool active = false;

	bool collided = false;

};