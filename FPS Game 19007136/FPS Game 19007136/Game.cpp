#include "Game.h"






void Game::Update()
{
	timer.RunTimer();
	camera.Shoot(devcon, timer.frameTime, hresult, dev);
}

void Game::DrawScene()
{
}

void Game::CreateBuffer(HRESULT& hresult_, ID3D11Device*& dev_, ID3D11DeviceContext*& devcon_)
{
	camera.CreateBuffer(hresult_, dev_);
	hresult = hresult_;
	dev = dev_;
	devcon = devcon_;
}


