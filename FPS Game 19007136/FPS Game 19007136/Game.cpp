#include "Game.h"






void Game::Update(int enemies, int& index_)
{
	index = index_;
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
		for (int i = 0; i < index_; i++)
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

	for (int i = 0; i < index_; i++)
	{
		camera.UpdateCamera(timer.frameTime, camera.cube[i].pos);
	}
	for (int e = 0; e < enemies; e++)
	{
		billboard[e].UpdateBillboard(timer.frameTime, camera.camPos);
	}
	UpdateScene();
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


void Game::WallCollision(std::vector<bool>& collidedFront, bool& collided)
{
	if (std::all_of(std::begin(collidedFront), std::end(collidedFront), [](bool i) {return !i; }))
	{
		collided = false;
	}
	for (int j = 0; j < collidedFront.size(); j++)
	{
		if (collidedFront.at(j) == true)
		{
			collidedFront.at(j) = false;
		}
	}
}

void Game::UpdateScene()
{
	WallCollision(collidedFront, camera.collidedFront);
	WallCollision(collidedBack, camera.collidedBack);
	WallCollision(collidedLeft, camera.collidedLeft);
	WallCollision(collidedRight, camera.collidedRight);
	for (int i = 0; i < index; i++)
	{
		if (camera.Intersecting(camera.cube[i].pos, camera.cube[i].scale))
		{
			camera.Resolve(camera.cube[i].pos, camera.cube[i].scale, collidedFront, collidedBack, collidedLeft, collidedRight, i);
		}
	}
}




void Game::CreateBuffer(HRESULT& hresult_, ID3D11Device*& dev_, ID3D11DeviceContext*& devcon_, HWND& hwnd_)
{
	camera.CreateBuffer(hresult_, dev_);
	hresult = hresult_;
	dev = dev_;
	devcon = devcon_;
	hwnd = hwnd_;
}


