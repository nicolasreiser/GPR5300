#pragma once
#include<d3d11.h>
#include<DirectXMath.h>

using namespace DirectX;
class Mesh
{
public:
	enum Object
	{
		Cube,
		Floor,
		Wall,
		Mirror
	};

	int init(ID3D11Device* pD3DDevice, Object object);
	void render(ID3D11DeviceContext* pD3DDeviceContext);
	void deInit();

private:
	int initVertexBuffer(ID3D11Device* pD3DDevice);
	int initIndexBuffer(ID3D11Device* pD3DDevice);

	Object _object;

	ID3D11Buffer* _pVertexBuffer = nullptr;
	ID3D11Buffer* _pIndexBuffer = nullptr;

	UINT _vertexCount = 0; // amount of vertices
	UINT _vertexStride = 0; // size of one vertex instance in bytes
	UINT _indexCount = 0; // amount of indices

};

