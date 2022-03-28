#include "StartMenu.h"
#include <string>
#include <iostream>

#pragma comment(lib, "d3d11.lib")

struct constantObjectBuffer
{
	dx::XMMATRIX WVP;
};

constantObjectBuffer constantObjBuffer_;

void StartMenu::DrawCube(ID3D11DeviceContext* deviceContext, float x, float y, float z,
	dx::XMMATRIX cameraView, dx::XMMATRIX cameraProjection)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;

	WVP = image * cameraView * cameraProjection;
	constantObjBuffer_.WVP = XMMatrixTranspose(WVP);
 	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &constantObjBuffer_, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);

	deviceContext->PSSetShaderResources(0, 1, &CubesTexture);

	deviceContext->DrawIndexed(6, 0, 0);

	position = dx::XMLoadFloat3(&pos);
}

void StartMenu::UpdateBillboard(double time, dx::XMFLOAT3 camPos)
{
	image = dx::XMMatrixIdentity();

	position = dx::XMLoadFloat3(&pos);

	float angle = atan2(pos.x - camPos.x, pos.z - camPos.z) * (180.0f / dx::XM_PI);
	float rotation_ = angle * 0.0174532925f;

	Rotation = dx::XMMatrixRotationY(rotation_);
	Translation = dx::XMMatrixTranslation(pos.x, pos.y, pos.z);
	Scale = dx::XMMatrixScaling(scale.x, scale.y, scale.z);

	image = Scale * Rotation * Translation;
}

void StartMenu::CreateBuffer(HRESULT hresult, ID3D11Device* device, const wchar_t* file)
{
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(constantObjectBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hresult = device->CreateBuffer(&cbbd, NULL, &constantBuffer);

	dx::ScratchImage image_data;
	hresult = dx::LoadFromWICFile(file, dx::WIC_FLAGS_NONE, nullptr, image_data);

	hresult = dx::CreateTexture(device, image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &textureResource);

	D3D11_SHADER_RESOURCE_VIEW_DESC imageDesc = {};
	imageDesc.Format = image_data.GetMetadata().format;
	imageDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	imageDesc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
	imageDesc.Texture2D.MostDetailedMip = 0;

	device->CreateShaderResourceView(textureResource, &imageDesc, &CubesTexture);
}

void StartMenu::InitDirectInput(HINSTANCE hInstance, HRESULT hresult, HWND hwnd)
{
	input.InitDirectInput(hInstance, hresult, hwnd);
}

void StartMenu::DetectInput(double time, HWND hwnd)
{
	input.DetectInput(time, hwnd);
}