#include "Bullet.h"
#include <string>

#pragma comment(lib, "d3d11.lib")

struct constantObjectBuffer
{
	dx::XMMATRIX WVP;
};

constantObjectBuffer _constantObjBuffer;


void Bullet::Shoot(ID3D11DeviceContext* deviceContext, double timer, HRESULT hresult, ID3D11Device* device, Inputs input, dx::XMMATRIX cameraView, dx::XMMATRIX cameraProjection, dx::XMFLOAT3 rotation, dx::XMFLOAT3 cameraPosition, dx::XMVECTOR cameraTarget)
{
	bullet.CreateBuffer(hresult, device);
	bullet.CreateTexture(hresult, device, L"Images/BulletImage.jpg");

	bullet.rotation = rotation.z * 3.157;
	bullet.pos = cameraPosition;

	dx::XMFLOAT3 camForward;
	dx::XMStoreFloat3(&camForward, cameraTarget);
	dx::XMVECTOR direction = { camForward.x - cameraPosition.x,
							   camForward.y - cameraPosition.y,
							   camForward.z - cameraPosition.z, 0 };

	dx::XMStoreFloat3(&result, direction);

	active = true;
	bulletPos = result;
}

void Bullet::DrawBullet(ID3D11DeviceContext* deviceContext, dx::XMMATRIX cameraView, dx::XMMATRIX cameraProjection)
{
	bullet.scale = dx::XMFLOAT3(.5,.75,.5);
	bullet.DrawCube(deviceContext, bullet.pos.x, bullet.pos.y, bullet.pos.z, cameraView, cameraProjection);
	bullet.pos.x = bullet.pos.x + bulletPos.x / 2;
	bullet.pos.z = bullet.pos.z + bulletPos.z / 2;
}

Bullet::Bullet()
{
	result = dx::XMFLOAT3(0, 0, 0);
	bulletPos = dx::XMFLOAT3(0,0,0);
	camProjection = dx::XMMATRIX();
	camView = dx::XMMATRIX();
}
