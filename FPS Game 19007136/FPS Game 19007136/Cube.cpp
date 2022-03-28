#include "Cube.h"

#pragma comment(lib, "d3d11.lib")

struct constantObjectBuffer
{
	dx::XMMATRIX WVP;
};

constantObjectBuffer constantObjBuffer;

Cube::Cube()
{

}

void Cube::DrawCube(ID3D11DeviceContext* deviceContext, float x,float y, float z,
		dx::XMMATRIX cameraView, dx::XMMATRIX cameraProjection)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
	

	cube = dx::XMMatrixIdentity();

	//Define cube1's world space matrix
	dx::XMVECTOR rotationAxis = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Rotation = dx::XMMatrixRotationAxis(rotationAxis, rotation);
	Translation = dx::XMMatrixTranslation(pos.x, pos.y, pos.z);
	Scale = dx::XMMatrixScaling(scale.x, scale.y, scale.z);

	cube = Scale * Rotation * Translation;

	WVP = cube * cameraView * cameraProjection;
	constantObjBuffer.WVP = XMMatrixTranspose(WVP);
	deviceContext->UpdateSubresource(constantBuffer, 0, NULL, &constantObjBuffer, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);

	deviceContext->PSSetShaderResources(0, 1, &CubesTexture);

	deviceContext->DrawIndexed(36, 0, 0);

	position = dx::XMLoadFloat3(&pos);
}

void Cube::CreateBuffer(HRESULT hresult, ID3D11Device* dev)
{
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(constantObjectBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hresult = dev->CreateBuffer(&cbbd, NULL, &constantBuffer);
}

void Cube::CreateTexture(HRESULT hresult, ID3D11Device* dev, const wchar_t* file)
{
	dx::ScratchImage image_data;
	hresult = dx::LoadFromWICFile(file, dx::WIC_FLAGS_NONE, nullptr, image_data);

	hresult = dx::CreateTexture(dev, image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &textureResource);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = image_data.GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
	desc.Texture2D.MostDetailedMip = 0;

	dev->CreateShaderResourceView(textureResource, &desc, &CubesTexture);
}