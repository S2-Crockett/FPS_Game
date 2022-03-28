#include "Graphics.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//Vertex Structure and Vertex Layout (Input Layout)//
struct Vertex	//Overloaded Vertex Structure
{
	Vertex() {}
	Vertex(float x, float y, float z,
		float u, float v)
		: pos(x, y, z), texCoord(u, v) {}

	dx::XMFLOAT3 pos;
	dx::XMFLOAT2 texCoord;
};

D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
};
UINT numElements = ARRAYSIZE(layout);


Vertex v[] =
{
	// Front Face
	Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
	Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
	Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
	Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

	// Back Face
	Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
	Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
	Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
	Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),

	// Top Face
	Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
	Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
	Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
	Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f),

	// Bottom Face
	Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
	Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
	Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
	Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),

	// Left Face
	Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
	Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
	Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
	Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

	// Right Face
	Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
	Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
	Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f),
	Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f),
};
Vertex v_[] =
{
	// Front Face
Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
};

DWORD indices[] = {
	// Front Face
	0,  1,  2,
	0,  2,  3,

	// Back Face
	4,  5,  6,
	4,  6,  7,

	// Top Face
	8,  9, 10,
	8, 10, 11,

	// Bottom Face
	12, 13, 14,
	12, 14, 15,

	// Left Face
	16, 17, 18,
	16, 18, 19,

	// Right Face
	20, 21, 22,
	20, 22, 23
};
DWORD indices_[] = {
	// Front Face
	0,  1,  2,
	0,  2,  3,
};



void Graphics::CleanUp()
{
	vertexBuffer->Release();
	indexBuffer->Release();
	VS->Release();
	PS->Release();
	VS_Buffer->Release();
	PS_Buffer->Release();
	vertLayout->Release();
}



void Graphics::CreateShaders(HRESULT hresult, ID3D11Device* device,
	ID3D11DeviceContext* deviceContext)
{

	hresult = D3DCompileFromFile(L"Effect.fx", 0, 0, "VS", "vs_4_0", 0, 0, &VS_Buffer, 0);
	hresult = D3DCompileFromFile(L"Effect.fx", 0, 0, "PS", "ps_4_0", 0, 0, &PS_Buffer, 0);


	hresult = device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hresult = device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);


	deviceContext->VSSetShader(VS, 0, 0);
	deviceContext->PSSetShader(PS, 0, 0);



	D3D11_BUFFER_DESC cubeIndexBufferDesc;
	ZeroMemory(&cubeIndexBufferDesc, sizeof(cubeIndexBufferDesc));

	cubeIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cubeIndexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	cubeIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	cubeIndexBufferDesc.CPUAccessFlags = 0;
	cubeIndexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subResData;

	subResData.pSysMem = indices;
	hresult = device->CreateBuffer(&cubeIndexBufferDesc, &subResData, &indexBuffer);

	D3D11_BUFFER_DESC billboardIndexBufferDesc;
	ZeroMemory(&billboardIndexBufferDesc, sizeof(billboardIndexBufferDesc));

	billboardIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	billboardIndexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
	billboardIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	billboardIndexBufferDesc.CPUAccessFlags = 0;
	billboardIndexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subResData_;

	subResData_.pSysMem = indices_;
	hresult = device->CreateBuffer(&billboardIndexBufferDesc, &subResData_, &indexBuffer_);


	stride = sizeof(Vertex);
	offset = 0;


	D3D11_BUFFER_DESC cubeVertexBufferDesc;
	ZeroMemory(&cubeVertexBufferDesc, sizeof(cubeVertexBufferDesc));

	cubeVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cubeVertexBufferDesc.ByteWidth = sizeof(Vertex) * 24;
	cubeVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	cubeVertexBufferDesc.CPUAccessFlags = 0;
	cubeVertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA cubeVertexBufferData;

	ZeroMemory(&cubeVertexBufferData, sizeof(cubeVertexBufferData));
	cubeVertexBufferData.pSysMem = v;
	hresult = device->CreateBuffer(&cubeVertexBufferDesc, &cubeVertexBufferData, &vertexBuffer);
	
	D3D11_BUFFER_DESC billboardVertexBufferDesc;
	ZeroMemory(&billboardVertexBufferDesc, sizeof(billboardVertexBufferDesc));

	billboardVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	billboardVertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
	billboardVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	billboardVertexBufferDesc.CPUAccessFlags = 0;
	billboardVertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA billboardVertexBufferData;

	ZeroMemory(&billboardVertexBufferData, sizeof(billboardVertexBufferData));
	billboardVertexBufferData.pSysMem = v_;
	hresult = device->CreateBuffer(&billboardVertexBufferDesc, &billboardVertexBufferData, &vertexBuffer_);


	//Create the Input Layout
	device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);

	//Set the Input Layout
	deviceContext->IASetInputLayout(vertLayout);

	//Set Primitive Topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Width;
	viewport.Height = Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//Set the Viewport
	deviceContext->RSSetViewports(1, &viewport);
}
