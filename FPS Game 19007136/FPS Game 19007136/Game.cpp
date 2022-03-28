#include "Game.h"

void Game::Update(int enemies, int& index_, int state)
{
	index = index_;
	timer.RunTimer();

	deviceContext->IASetIndexBuffer(GFX.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetVertexBuffers(0, 1, &GFX.vertexBuffer, &GFX.stride, &GFX.offset);

	if (state == 1)
	{
		
	camera.DetectInput(timer.frameTime, hwnd);

	for (auto& bullets : bullet_)
	{
		for (int e = 0; e < enemies; e++)
		{
			if (Collision(bullets->bullet.pos, billboard_.at(e).get()->pos) &&
				billboard_.at(e).get()->active)
			{
				bullets->active = false;
				billboard_.at(e).get()->active = false;
				enemiesDead += 1;
			}
		}
		for (int i = 0; i < index_; i++)
		{
			if (Collision(bullets->bullet.pos, camera.cube[i].pos))
			{
				bullets->active = false;
			}
		}
	}

	for (int i = 0; i < index_; i++)
	{
		camera.UpdateCamera(timer.frameTime, camera.cube[i].pos);
	}
	for (int e = 0; e < enemies; e++)
	{
		billboard_.at(e).get()->UpdateEnemy(timer.frameTime, camera.cameraPos);
	}
		UpdateScene();
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
	Shoot(deviceContext, timer.frameTime, hresult, device);
}


void Game::WallCollision(std::vector<bool>& collidedFront, bool& collided)
{
	if (std::all_of(std::begin(collidedFront), std::end(collidedFront), [](bool i) {return !i;}))
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


void Game::Shoot(ID3D11DeviceContext* deviceContext, double timer, HRESULT hresult, ID3D11Device* device)
{
	dx::XMFLOAT3 test(0, 0, 0);
	if (camera.input.shoot)
	{
		if (!shot)
		{
			bullerIndex += 1;
			bullet_.emplace_back(new Bullet);
			bullet_.at(bullerIndex) = std::make_unique<Bullet>();

			bullet_.at(bullerIndex)->Shoot(deviceContext, timer, hresult, device, camera.input, camera.cameraView, camera.cameraProjection, camera.rotation, camera.cameraPos, camera.cameraTarget);
			shot = true;
		}
	}
	else
	{
		dx::XMVECTOR test;
		test = dx::XMLoadFloat3(&camera.rotation);
		dx::XMVector3Normalize(test);
		shot = false;
	}
	for (auto& bullets : bullet_)
	{
		if (bullets->active)
		{
			bullets->DrawBullet(deviceContext, camera.cameraView, camera.cameraProjection);
		}
	}
}




void Game::CreateBuffer(HRESULT& hresult_, ID3D11Device*& device_, ID3D11DeviceContext*& deviceContext_, HWND& hwnd_)
{
	camera.CreateBuffer(hresult_, device_);
	hresult = hresult_;
	device = device_;
	deviceContext = deviceContext_;
	hwnd = hwnd_;
}


