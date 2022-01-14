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

	void Update(int enemies, int index);

	void DrawScene();
	void CreateBuffer(HRESULT& hresult, ID3D11Device*& dev, ID3D11DeviceContext*& devcon, HWND& hwnd);

	bool Collision(dx::XMFLOAT3 bullet, dx::XMFLOAT3 target);


	BillBoard billboard[2];

	int enemiesDead = 0;
};



