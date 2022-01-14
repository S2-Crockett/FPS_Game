#include "Game.h"






void Game::Update(int enemies, int index)
{
	timer.RunTimer();
	camera.Shoot(devcon, timer.frameTime, hresult, dev);
	camera.DetectInput(timer.frameTime, hwnd);

	for (auto& bullets : camera.bullet)
	{
		for (int e = 0; e < enemies; e++)
		{
			if (Collision(bullets->pos, billboard[e].pos) &&
				billboard[e].active)
			{
				bullets->active = false;
				billboard[e].active = false;
				enemiesDead += 1;
			}
		}
		for (int i = 0; i < index; i++)
		{
			if (Collision(bullets->pos, camera.cube[i].pos))
			{
				bullets->active = false;
			}
		}
	}

	if (enemiesDead == sizeof(billboard) / sizeof(*billboard))
	{
		DestroyWindow(hwnd);
	}

	for (int i = 0; i < index; i++)
	{
		camera.UpdateCamera(timer.frameTime, camera.cube[i].pos);
	}
	for (int e = 0; e < enemies; e++)
	{
		billboard[e].UpdateBillboard(timer.frameTime, camera.camPos);
	}
}

bool Game::Collision(dx::XMFLOAT3 bullet, dx::XMFLOAT3 target)
{
	return bullet.x < target.x + 3 &&
		bullet.x > target.x - 3 &&
		bullet.z < target.z + 3 &&
		bullet.z > target.z - 3;
}

void Game::DrawScene()
{
}

void Game::CreateBuffer(HRESULT& hresult_, ID3D11Device*& dev_, ID3D11DeviceContext*& devcon_, HWND& hwnd_)
{
	camera.CreateBuffer(hresult_, dev_);
	hresult = hresult_;
	dev = dev_;
	devcon = devcon_;
	hwnd = hwnd_;
}


