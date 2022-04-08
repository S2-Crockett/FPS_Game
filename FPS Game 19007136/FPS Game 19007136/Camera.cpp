#include "Camera.h"
#include <string>
#include <iostream>
using namespace std;




#pragma comment(lib, "d3d11.lib")

Camera::Camera()
{

}

void Camera::CreateBuffer(HRESULT hresult, ID3D11Device* device)
{
	for (int i = 0; i < width * height; i++)
	{
		cube[i].CreateBuffer(hresult, device);
	}
	for (int i = 0; i < width * height; i++)
	{
		cube[i].CreateTexture(hresult, device, L"Images/Brick.jpg");
	}

	floorCube.CreateBuffer(hresult, device);
	floorCube.CreateTexture(hresult, device, L"Images/Ground.jpg");

	//Camera information
	cameraPosition = dx::XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f);
	cameraTarget = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	cameraUpDir = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//Set the View matrix
	cameraView = dx::XMMatrixLookAtLH(cameraPosition, cameraTarget, cameraUpDir);

	//Set the Projection matrix
	cameraProjection = dx::XMMatrixPerspectiveFovLH(0.4f * 3.14f, Width / Height, 1.0f, 1000.0f);
}

void Camera::DrawCube(ID3D11DeviceContext* deviceContext, float x, float y, float z, int index)
{
	dx::XMFLOAT3 Scale(2, 8, 2);
	cube[index].scale = Scale;
	cube[index].DrawCube(deviceContext, x, y + 5, z, cameraView, cameraProjection);
}

void Camera::DrawFloorCube(ID3D11DeviceContext* deviceContext, float x, float y, float z, int index)
{
	dx::XMFLOAT3 Scale(150, 2, 50);
	floorCube.scale = Scale;
	floorCube.DrawCube(deviceContext, x, y, z, cameraView, cameraProjection);
	floorCube.DrawCube(deviceContext, x, y + 20.0f, z, cameraView, cameraProjection);
}



void Camera::Shoot(ID3D11DeviceContext* deviceContext, double timer, HRESULT hresult, ID3D11Device* device)
{
	dx::XMFLOAT3 test(0, 0, 0);
	if (input.shoot)
	{
		if (!shot)
		{
			index += 1;
			bullet_.emplace_back(new Bullet);
			bullet_.at(index) = std::make_unique<Bullet>();

			bullet_.at(index)->Shoot(deviceContext, timer, hresult, device, input, cameraView, cameraProjection, rotation, cameraPos, cameraTarget);
			shot = true;
		}
	}
	else
	{
		dx::XMVECTOR test;
		test = dx::XMLoadFloat3(&rotation);
		dx::XMVector3Normalize(test);
		shot = false;
	}
	for (auto& bullets : bullet_)
	{
		if (bullets->active)
		{
			bullets->DrawBullet(deviceContext, cameraView, cameraProjection);
		}
	}
}


void Camera::UpdateCamera(double time, dx::XMFLOAT3 pos_)
{

	dx::XMVECTOR test = dx::XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f);
	dx::XMFLOAT3 test2 = { pos_.x,pos_.y,pos_.z };
	dx::XMVECTOR speed = dx::XMVectorSet(250.0f * time, 0.0f, 250.0f * time, 0.0f);


	input.cameraRotationMatrix = dx::XMMatrixRotationRollPitchYaw(0, input.cameraYaw, 0);
	cameraTarget = dx::XMVector3TransformCoord(input.baseForward, input.cameraRotationMatrix);
	cameraTarget = dx::XMVector3Normalize(cameraTarget);

	dx::XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = dx::XMMatrixRotationY(input.cameraYaw);

	cameraUpDir = XMVector3TransformCoord(cameraUpDir, RotateYTempMatrix);
	input.cameraForward = XMVector3TransformCoord(input.baseForward, RotateYTempMatrix);


	dx::XMFLOAT3 forwardBack;
	dx::XMStoreFloat3(&forwardBack, test);

	float dot = { (forwardBack.x * test2.x) + (forwardBack.y * test2.y) + (forwardBack.z * test2.z) };
	float x = test2.x * dot;
	float y = test2.y * dot;
	float z = test2.z * dot;


	if (collidedFront || collidedBack || collidedRight || collidedLeft)
	{

		if (collidedFront)
		{
			//forwardBack.x += x;
			//forwardBack.y -= y;
			forwardBack.z += z;
			test = dx::XMLoadFloat3(&forwardBack);
			test = dx::XMVector3Normalize(test);
		}
		else if (collidedBack)
		{
			//forwardBack.x -= x;
			//forwardBack.y -= y;
			forwardBack.z -= z;
			test = dx::XMLoadFloat3(&forwardBack);
			test = dx::XMVector3Normalize(test);
		}
		else if (collidedRight)
		{
			//forwardBack.z -= z;
			//forwardBack.y -= y;
			forwardBack.x += x;
			test = dx::XMLoadFloat3(&forwardBack);
			test = dx::XMVector3Normalize(test);
		}
		else if (collidedLeft)
		{
			//forwardBack.z -= z;
			//forwardBack.y -= y;
			forwardBack.x -= x;
			test = dx::XMLoadFloat3(&forwardBack);
			test = dx::XMVector3Normalize(test);
		}
		cameraPosition = dx::XMVectorAdd(cameraPosition, dx::XMVectorMultiply(input.moveForward, dx::XMVectorMultiply(test, speed)));
	}
	else
	{
		cameraPosition = dx::XMVectorAdd(cameraPosition, dx::XMVectorMultiply(input.moveForward, input.cameraForward));
	}


	input.moveForward = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	dx::XMVECTOR target = dx::XMVectorAdd(cameraPosition, cameraTarget);
	cameraTarget = target;

	cameraView = dx::XMMatrixLookAtLH(cameraPosition, cameraTarget, cameraUpDir);

	dx::XMVECTOR offset = dx::XMVectorSet(-1.0f, -1.0f, -1.0f, -1.0f);
	dx::XMStoreFloat3(&cameraPos, cameraPosition);
}




bool Camera::Intersecting(dx::XMFLOAT3 pos, dx::XMFLOAT3 scale)
{
	return (cameraPos.x <= pos.x + scale.x + 4.0f &&
		cameraPos.x >= pos.x - scale.x - 4.0f &&
		cameraPos.z >= pos.z - scale.z - 4.0f &&
		cameraPos.z <= pos.z + scale.z + 4.0f);
}

void Camera::Resolve(dx::XMFLOAT3 pos_, dx::XMFLOAT3 scale_, std::vector<bool>& collidedFront_, std::vector<bool>& collidedBack_, std::vector<bool>& collidedLeft_, std::vector<bool>& collidedRight_, int index)
{
	dx::XMFLOAT3 objMin = { pos_.x - scale_.x, pos_.y, pos_.z };
	dx::XMFLOAT3 objMax = { pos_.x + scale_.x, pos_.y + scale_.y, pos_.z + scale_.z };
	dx::XMFLOAT3 thisMin = { cameraPos.x - scale.x, cameraPos.y, cameraPos.z };
	dx::XMFLOAT3 thisMax = { cameraPos.x + scale.x, cameraPos.y + scale.y, cameraPos.z + scale.z };

	if (thisMax.x < objMin.x)
	{
		collidedRight = true;
		collidedRight_.emplace_back(true);
	}
	else if (thisMin.x > objMax.x)
	{
		collidedLeft = true;
		collidedLeft_.emplace_back(true);
	}
	else if (thisMax.z < objMin.z)
	{
		collidedBack = true;
		collidedBack_.emplace_back(true);
	}
	else if (thisMin.z > objMax.z)
	{
		collidedFront = true;
		collidedFront_.emplace_back(true);
	}
}

void Camera::InitDirectInput(HINSTANCE hInstance, HRESULT hresult, HWND hwnd)
{
	input.InitDirectInput(hInstance, hresult, hwnd);
}

void Camera::DetectInput(double time, HWND hwnd)
{
	input.DetectInput(time, hwnd);
}