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

void Graphics::CreateShaders(HRESULT hresult, ID3D11Device* dev,
	ID3D11DeviceContext* devcon)
{

	hresult = D3DCompileFromFile(L"Effect.fx", 0, 0, "VS", "vs_4_0", 0, 0, &VS_Buffer, 0);
	hresult = D3DCompileFromFile(L"Effect.fx", 0, 0, "PS", "ps_4_0", 0, 0, &PS_Buffer, 0);


	//Create the Shader Objects
	hresult = dev->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hresult = dev->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

	//Set Vertex and Pixel Shaders
	devcon->VSSetShader(VS, 0, 0);
	devcon->PSSetShader(PS, 0, 0);



	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	hresult = dev->CreateBuffer(&indexBufferDesc, &iinitData, &indexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc_;
	ZeroMemory(&indexBufferDesc_, sizeof(indexBufferDesc_));

	indexBufferDesc_.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc_.ByteWidth = sizeof(DWORD) * 2 * 3;
	indexBufferDesc_.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc_.CPUAccessFlags = 0;
	indexBufferDesc_.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData_;

	iinitData_.pSysMem = indices_;
	hresult = dev->CreateBuffer(&indexBufferDesc_, &iinitData_, &indexBuffer_);





	stride = sizeof(Vertex);
	offset = 0;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 24;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	hresult = dev->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);
	
	D3D11_BUFFER_DESC vertexBufferDesc_;
	ZeroMemory(&vertexBufferDesc_, sizeof(vertexBufferDesc_));

	vertexBufferDesc_.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc_.ByteWidth = sizeof(Vertex) * 4;
	vertexBufferDesc_.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc_.CPUAccessFlags = 0;
	vertexBufferDesc_.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData_;

	ZeroMemory(&vertexBufferData_, sizeof(vertexBufferData_));
	vertexBufferData_.pSysMem = v_;
	hresult = dev->CreateBuffer(&vertexBufferDesc_, &vertexBufferData_, &vertexBuffer_);


	//Create the Input Layout
	dev->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);

	//Set the Input Layout
	devcon->IASetInputLayout(vertLayout);

	//Set Primitive Topology
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
	devcon->RSSetViewports(1, &viewport);

	camera.CreateBuffer(hresult, dev);
}
