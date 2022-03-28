#include "BillBoard.h"
#include <string>
#include <iostream>

#pragma comment(lib, "d3d11.lib")

struct constantObjectBuffer
{
	dx::XMMATRIX WVP;
};

constantObjectBuffer constantObjBuffer__;

void BillBoard::DrawEnemy(ID3D11DeviceContext* deviceContext, float x, float y, float z,
	dx::XMMATRIX cameraView, dx::XMMATRIX cameraProjection)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;

	WVP = cube * cameraView * cameraProjection;
	constantObjBuffer__.WVP = XMMatrixTranspose(WVP);
	deviceContext->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &constantObjBuffer__, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

	deviceContext->PSSetShaderResources(0, 1, &cubeTextureView);

	deviceContext->DrawIndexed(6, 0, 0);

	position = dx::XMLoadFloat3(&pos);
} 

void BillBoard::UpdateEnemy(double time, dx::XMFLOAT3 camPos)
{
	cube = dx::XMMatrixIdentity();
	
	position = dx::XMLoadFloat3(&pos);

	float angle = atan2(pos.x - camPos.x, pos.z - camPos.z) * (180.0f / dx::XM_PI);
	float rotation_ = angle * 0.0174532925f;

	Rotation = dx::XMMatrixRotationY(rotation_);
	Translation = dx::XMMatrixTranslation(pos.x, pos.y, pos.z);
	Scale = dx::XMMatrixScaling(scale.x, 6.0f, scale.z);
	
	cube = Scale * Rotation * Translation;
}

void BillBoard::CreateBuffer(HRESULT hresult, ID3D11Device* dev, const wchar_t* file)
{
	D3D11_BUFFER_DESC constantBuffer;
	ZeroMemory(&constantBuffer, sizeof(D3D11_BUFFER_DESC));

	constantBuffer.Usage = D3D11_USAGE_DEFAULT;
	constantBuffer.ByteWidth = sizeof(constantObjectBuffer);
	constantBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBuffer.CPUAccessFlags = 0;
	constantBuffer.MiscFlags = 0;

	hresult = dev->CreateBuffer(&constantBuffer, NULL, &cbPerObjectBuffer);

	dx::ScratchImage imageData;
	hresult = dx::LoadFromWICFile(file, dx::WIC_FLAGS_NONE, nullptr, imageData);

	hresult = dx::CreateTexture(dev, imageData.GetImages(), imageData.GetImageCount(), imageData.GetMetadata(), &textureReasource);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = imageData.GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = imageData.GetMetadata().mipLevels;
	desc.Texture2D.MostDetailedMip = 0;

	dev->CreateShaderResourceView(textureReasource, &desc, &cubeTextureView);
}


 
