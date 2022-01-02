#include "Bullet.h"

#pragma comment(lib, "d3d11.lib")

struct cbPerObject
{
	dx::XMMATRIX WVP;
};

cbPerObject _cbPerObj;




void Bullet::CreateBullet(HRESULT hresult, ID3D11Device* dev, const wchar_t* file, ID3D11DeviceContext* devcon, dx::XMFLOAT3 pos, dx::XMMATRIX camView, dx::XMMATRIX camProjection)
{
	bullet->CreateBuffer(hresult, dev);
	bullet->CreateTexture(hresult, dev, file);
	bullet->DrawCube(devcon, pos.x, pos.y, pos.z, camView, camProjection);
}


Bullet::Bullet()
{
}
