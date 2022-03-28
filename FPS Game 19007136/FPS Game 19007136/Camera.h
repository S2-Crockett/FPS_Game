#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include "Inputs.h"
#include "Bullet.h"
#include <vector>
#include <algorithm>
#include <string>
namespace dx = DirectX;

class Camera
{

public:
	Camera();
	~Camera() = default;

	Inputs input;

	static const int width = 20;
	static const int height = 20;
	Cube cube[width * height];
	Cube floorCube;
	std::vector<std::unique_ptr<Bullet>> bullet_;

	//void CleanUp();
	void CreateBuffer(HRESULT hresult, ID3D11Device* device);

	void DrawCube(ID3D11DeviceContext* deviceContext, float x, float y, float z, int index);
	void DrawFloorCube(ID3D11DeviceContext* deviceContext, float x, float y, float z, int index);
	void Shoot(ID3D11DeviceContext* deviceContext, double timer, HRESULT hresult, ID3D11Device* device);
	void UpdateCamera(double time, dx::XMFLOAT3 pos_);

	float y;

	bool Intersecting(dx::XMFLOAT3 pos_, dx::XMFLOAT3 scale);
	void Resolve(dx::XMFLOAT3 pos_, dx::XMFLOAT3 scale_, std::vector<bool> &_collided, std::vector<bool>& collidedBack_, std::vector<bool>& collidedLeft_, std::vector<bool>& collidedRight_, int index);

	void InitDirectInput(HINSTANCE hInstance, HRESULT hresult, HWND hwnd);
	void DetectInput(double time, HWND hwnd);

	dx::XMFLOAT3 cameraPos = { 30.0f, 3.0f, -30.0f };
	dx::XMFLOAT3 scale = { 2.0f, 2.0f, 2.0f };
	dx::XMFLOAT3 rotation;
	dx::XMMATRIX rotation_;
	dx::XMVECTOR cameraPosition;

	bool collidedFront = false;
	bool collidedBack = false;
	bool collidedRight = false;
	bool collidedLeft = false;

	dx::XMMATRIX cameraView;
	dx::XMMATRIX cameraProjection;

	dx::XMVECTOR cameraTarget;
	dx::XMVECTOR cameraUpDir;

private:

	dx::XMMATRIX World;

	dx::XMFLOAT3 camTarget_;


	int index = -1;
	bool shot = false;
	float rot = 0.0f;

	const int Width = 800;
	const int Height = 600;

	dx::XMFLOAT3 result;
};

