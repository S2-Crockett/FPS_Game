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

	void Update();

	void DrawScene();
	void CreateBuffer(HRESULT& hresult, ID3D11Device*& dev, ID3D11DeviceContext*& devcon);


	BillBoard billboard[2];
};



