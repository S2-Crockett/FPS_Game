#pragma once

#include "Timer.h"
#include "Graphics.h"
#include "Camera.h"
#include <fstream>

using namespace std;

class Game
{
public:
	Timer timer;
	Camera camera;
	Graphics GFX;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	HRESULT hresult;
	HWND hwnd = NULL;

	void Update(int enemies, int& index, int state);
	void UpdateScene();

	void DrawScene();
	void CreateBuffer(HRESULT& hresult, ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, HWND& hwnd);

	bool Collision(dx::XMFLOAT3 bullet, dx::XMFLOAT3 target);
	void WallCollision(std::vector<bool>& collidedFront, bool& collided);

	void Shoot(ID3D11DeviceContext* devcon, double timer, HRESULT hresult, ID3D11Device* device);

	std::vector<std::unique_ptr<BillBoard>> billboard_;

	int enemiesDead = 0;
	int index = 0;
	int bullerIndex = -1;
	bool shot = false;

	std::vector<std::unique_ptr<Bullet>> bullet_;

	std::vector<bool> collidedFront, collidedBack, collidedLeft, collidedRight;
};



