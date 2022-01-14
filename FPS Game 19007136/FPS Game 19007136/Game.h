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

	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;
	HRESULT hresult;
	HWND hwnd = NULL;

	void Update(int enemies, int& index);
	void UpdateScene();

	void DrawScene();
	void CreateBuffer(HRESULT& hresult, ID3D11Device*& dev, ID3D11DeviceContext*& devcon, HWND& hwnd);

	bool Collision(dx::XMFLOAT3 bullet, dx::XMFLOAT3 target);
	void WallCollision(std::vector<bool>& collidedFront, bool& collided);


	BillBoard billboard[2];

	int enemiesDead = 0;
	int index = 0;

	std::vector<bool> collidedFront, collidedBack, collidedLeft, collidedRight;
};



