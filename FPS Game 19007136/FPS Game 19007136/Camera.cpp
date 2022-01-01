#include "Camera.h"
#include <string>
#include <iostream>


#pragma comment(lib, "d3d11.lib")

Camera::Camera()
{
	bullet.emplace_back(new Cube);
	bullet.at(0) = std::make_unique<Cube>();
}

void Camera::CreateBuffer(HRESULT hresult, ID3D11Device* dev)
{
	for (int i = 0; i < width * height; i++)
	{
		cube[i].CreateBuffer(hresult, dev);
		floorCube[i].CreateBuffer(hresult, dev);
	}
	for (int i = 0; i < width * height; i++)
	{
		cube[i].CreateTexture(hresult, dev, L"Image.jpg");
		floorCube[i].CreateTexture(hresult, dev, L"Image2.jpg");
	}


	bullet.at(0)->CreateBuffer(hresult, dev);
	bullet.at(0)->CreateTexture(hresult, dev, L"Image.jpg");

	//Camera information
	camPosition = dx::XMVectorSet(camPos.x, camPos.y, camPos.z, 0.0f);
	camTarget = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//Set the View matrix
	camView = dx::XMMatrixLookAtLH(camPosition, camTarget, camUp);

	//Set the Projection matrix
	camProjection = dx::XMMatrixPerspectiveFovLH(0.4f * 3.14f, Width / Height, 1.0f, 1000.0f);



}

void Camera::DrawCube(ID3D11DeviceContext* devcon, float x, float y, float z, int index)
{
	cube[index].DrawCube(devcon, x, y, z, camView, camProjection);
	cube[index].DrawCube(devcon, x, y + 5.0f, z, camView, camProjection);
	cube[index].DrawCube(devcon, x, y + 10.0f, z, camView, camProjection);
}
void Camera::DrawFloorCube(ID3D11DeviceContext* devcon, float x, float y, float z, int index)
{
	floorCube[index].DrawCube(devcon, x, y, z, camView, camProjection);
	floorCube[index].DrawCube(devcon, x, y + 20.0f, z, camView, camProjection);

	bullet.at(0)->DrawCube(devcon, bullet.at(0)->pos.x, bullet.at(0)->pos.y, bullet.at(0)->pos.z, camView, camProjection);
}



void Camera::Shoot(ID3D11DeviceContext* devcon, double timer)
{
		if (input.shoot)
		{
			if (!shot)
			{
					bullet.at(index)->rot = rotation.z * 3.157;
					bullet.at(index)->pos = camPos;
					shot = true;
			}
			dx::XMFLOAT3 test_;
			dx::XMStoreFloat3(&test_, camTarget);

			dx::XMVECTOR _test = { test_.x - camPos.x,
								 test_.y - camPos.y,
								 test_.z - camPos.z,0};
			dx::XMVECTOR direction = dx::XMVectorSet(10.0f, 0.0f, 0.0f, 0.0f);
			dx::XMVECTOR facing;

			
		
			facing = dx::XMVector3Dot(direction, _test);
			dx::XMFLOAT3 result;
			dx::XMStoreFloat3(&result, _test);

			bullet.at(index)->pos.x = bullet.at(index)->pos.x + result.x / 2;
			bullet.at(index)->pos.z = bullet.at(index)->pos.z + result.z / 2;
		}
		else
		{
			dx::XMVECTOR test;
			test = dx::XMLoadFloat3(&rotation);
			dx::XMVector3Normalize(test);
		}
}


void Camera::UpdateCamera(double time, dx::XMFLOAT3 pos_)
{

	dx::XMVECTOR test = dx::XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f);
	dx::XMFLOAT3 test2 = { pos_.x,pos_.y,pos_.z };
	dx::XMVECTOR speed = dx::XMVectorSet(250.0f * time, 0.0f, 250.0f * time, 0.0f);


		input.camRotationMatrix = dx::XMMatrixRotationRollPitchYaw(0, input.camYaw, 0);
		camTarget = dx::XMVector3TransformCoord(input.DefaultForward, input.camRotationMatrix);
		camTarget = dx::XMVector3Normalize(camTarget);

		dx::XMMATRIX RotateYTempMatrix;
		RotateYTempMatrix = dx::XMMatrixRotationY(input.camYaw);

		input.camRight = XMVector3TransformCoord(input.DefaultRight, RotateYTempMatrix);
		camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
		input.camForward = XMVector3TransformCoord(input.DefaultForward, RotateYTempMatrix);


			dx::XMFLOAT3 forwardBack;
			dx::XMStoreFloat3(&forwardBack, test);

			float dot = {(forwardBack.x * test2.x) + (forwardBack.y * test2.y) + (forwardBack.z * test2.z)};
			float x = test2.x * dot;
			float y = test2.y * dot; 
			float z = test2.z * dot;



		
		if (collidedFront || collidedBack || collidedRight || collidedLeft)
		{
			if (collidedFront)
			{
				//forwardBack.x -= x;
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
			camPosition = dx::XMVectorAdd(camPosition, dx::XMVectorMultiply(input.moveBackForward, dx::XMVectorMultiply(test, speed)));
		}
		else
		{
			camPosition = dx::XMVectorAdd(camPosition, dx::XMVectorMultiply(input.moveBackForward, input.camForward));
			camPosition = dx::XMVectorAdd(camPosition, dx::XMVectorMultiply(input.moveLeftRight, input.camRight));
		}
		


		input.moveLeftRight = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		input.moveBackForward = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		dx::XMVECTOR target = dx::XMVectorAdd(camPosition, camTarget);
		camTarget = target;

		camView = dx::XMMatrixLookAtLH(camPosition, camTarget, camUp);

		dx::XMVECTOR offset = dx::XMVectorSet(-1.0f, -1.0f, -1.0f, -1.0f);
		dx::XMStoreFloat3(&camPos, camPosition);
		dx::XMStoreFloat3(&rotation, dx::XMVectorMultiply(input.camRight, offset));
}




bool Camera::Intersecting(dx::XMFLOAT3 pos, dx::XMFLOAT3 scale)
{
	return (camPos.x <= pos.x + scale.x + 4.0f &&
		camPos.x >= pos.x - scale.x - 4.0f &&
		camPos.z >= pos.z - scale.z - 4.0f &&
		camPos.z <= pos.z + scale.z + 4.0f);
}

void Camera::Resolve(dx::XMFLOAT3 pos_, dx::XMFLOAT3 scale_, std::vector<bool> &collidedFront_, std::vector<bool>& collidedBack_, std::vector<bool>& collidedLeft_, std::vector<bool>& collidedRight_, int index)
{
	dx::XMFLOAT3 objMin = { pos_.x - scale_.x, pos_.y, pos_.z };
	dx::XMFLOAT3 objMax = { pos_.x + scale_.x, pos_.y + scale_.y, pos_.z +scale_.z};
	dx::XMFLOAT3 thisMin = { camPos.x - scale.x, camPos.y, camPos.z };
	dx::XMFLOAT3 thisMax = { camPos.x + scale.x, camPos.y + scale.y, camPos.z + scale.z };

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