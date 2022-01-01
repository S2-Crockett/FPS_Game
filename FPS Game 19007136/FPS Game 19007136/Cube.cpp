#include "Cube.h"

#pragma comment(lib, "d3d11.lib")

struct cbPerObject
{
	dx::XMMATRIX WVP;
};

cbPerObject cbPerObj;

Cube::Cube()
{

}

void Cube::DrawCube(ID3D11DeviceContext* devcon, float x,float y, float z,
		dx::XMMATRIX camView, dx::XMMATRIX camProjection)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
	

	cube1World = dx::XMMatrixIdentity();

	//Define cube1's world space matrix
	dx::XMVECTOR rotaxis = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Rotation = dx::XMMatrixRotationAxis(rotaxis, rot);
	Translation = dx::XMMatrixTranslation(pos.x, pos.y, pos.z);
	Scale = dx::XMMatrixScaling(scale.x, scale.y, scale.z);

	cube1World = Scale * Rotation * Translation;

	WVP = cube1World * camView * camProjection;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	devcon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	devcon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

	devcon->PSSetShaderResources(0, 1, &CubesTexture);

	devcon->DrawIndexed(36, 0, 0);

	position = dx::XMLoadFloat3(&pos);
}

void Cube::CreateBuffer(HRESULT hresult, ID3D11Device* dev)
{
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hresult = dev->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
}

void Cube::CreateTexture(HRESULT hresult, ID3D11Device* dev, const wchar_t* file)
{
	dx::ScratchImage image_data;
	hresult = dx::LoadFromWICFile(file, dx::WIC_FLAGS_NONE, nullptr, image_data);

	hresult = dx::CreateTexture(dev, image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &m_texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = image_data.GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
	desc.Texture2D.MostDetailedMip = 0;

	dev->CreateShaderResourceView(m_texture, &desc, &CubesTexture);
}