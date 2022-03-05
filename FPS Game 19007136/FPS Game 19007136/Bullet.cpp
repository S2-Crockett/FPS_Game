#include "Bullet.h"

#pragma comment(lib, "d3d11.lib")

struct cbPerObject
{
	dx::XMMATRIX WVP;
};

cbPerObject _cbPerObj;




void Bullet::CreateBullet(HRESULT hresult, ID3D11Device* dev, const wchar_t* file, ID3D11DeviceContext* devcon, dx::XMFLOAT3 pos, dx::XMMATRIX _camView, dx::XMMATRIX _camProjection)
{
	bullet.CreateBuffer(hresult, dev);
	bullet.CreateTexture(hresult, dev, L"Image.jpg");
}


void Bullet::Shoot(ID3D11DeviceContext* devcon, double timer, HRESULT hresult, ID3D11Device* dev, Inputs input, dx::XMMATRIX _camView, dx::XMMATRIX _camProjection, dx::XMFLOAT3 rotation, dx::XMFLOAT3 camPos, dx::XMVECTOR camTarget)
{
	bullet.CreateBuffer(hresult, dev);
	bullet.CreateTexture(hresult, dev, L"Image.jpg");

	bullet.rot = rotation.z * 3.157;
	bullet.pos = camPos;

	dx::XMFLOAT3 camForward;
	dx::XMStoreFloat3(&camForward, camTarget);
	dx::XMVECTOR direction = { camForward.x - camPos.x,
							   camForward.y - camPos.y,
							   camForward.z - camPos.z, 0 };


	dx::XMStoreFloat3(&result, direction);

	active = true;
	bulletPos = result;
}

void Bullet::DrawBullet(ID3D11DeviceContext* devcon, dx::XMMATRIX _camView, dx::XMMATRIX _camProjection)
{
	bullet.DrawCube(devcon, bullet.pos.x, bullet.pos.y, bullet.pos.z, _camView, _camProjection);
	bullet.pos.x = bullet.pos.x + bulletPos.x / 2;
	bullet.pos.z = bullet.pos.z + bulletPos.z / 2;
}

Bullet::Bullet()
{
}
