#include "Mesh.h"
#include "Vertex.h"
#include "Utilities.h"


int Mesh::init(ID3D11Device* pD3DDevice, Object object)
{
	_object = object;

	int error = initVertexBuffer(pD3DDevice);
	if (error != 0) return error;

	error = initIndexBuffer(pD3DDevice);
	if (error != 0) return error;
	
}

void Mesh::render(ID3D11DeviceContext* pD3DDeviceContext)
{
		static UINT offset = 0;

		pD3DDeviceContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &_vertexStride, &offset);
		pD3DDeviceContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		pD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pD3DDeviceContext->DrawIndexed(_indexCount, 0, 0);
}

void Mesh::deInit()
{
	safeRelease<ID3D11Buffer>(_pVertexBuffer);
	safeRelease<ID3D11Buffer>(_pIndexBuffer);
}

int Mesh::initVertexBuffer(ID3D11Device* pD3DDevice)
{
	
	switch (_object)
	{
	case Mesh::Cube:
	{
		_vertexCount = 24;
		_vertexStride = sizeof(Vertex);

		Vertex vertices[] =
		{
			// quad - trianglestrip or trianglelist with index buffer
			// position with normal & uv

			//front side
			Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f),
			Vertex(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f),
			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f),
			Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f),

			//Right Side
			Vertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
			Vertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f),
			Vertex(0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),

			//Left Side
			Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
			Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f),
			Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f),
			Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),

			//Back Side
			Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),
			Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
			Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
			Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),

			//Top Side
			Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
			Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
			Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, -0.0f),

			//Bot Side
			Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f),
			Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f),
			Vertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f),
			Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f)
		};

		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = _vertexCount * _vertexStride;
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA initialData = {};
		initialData.pSysMem = vertices;

		HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pVertexBuffer);
		if (FAILED(hr)) return 30;

		return 0;
	}
	case Mesh::Floor:
	{
		_vertexCount = 4;
		_vertexStride = sizeof(Vertex);

		Vertex vertices[] =
		{
			// quad - trianglestrip or trianglelist with index buffer
			// position with normal & uv

			//floor
			Vertex(-0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
			Vertex(0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
			Vertex(-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
			Vertex(0.5f, 0.0f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f),


		};

		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = _vertexCount * _vertexStride;
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA initialData = {};
		initialData.pSysMem = vertices;

		HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pVertexBuffer);
		if (FAILED(hr)) return 30;

		return 0;
	}
	case Mesh::Wall:
	{
		_vertexCount = 4;
		_vertexStride = sizeof(Vertex);

		Vertex vertices[] =
		{
			// quad - trianglestrip or trianglelist with index buffer
			// position with normal & uv

			//floor
			Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
			Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f),
			Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),


		};

		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = _vertexCount * _vertexStride;
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA initialData = {};
		initialData.pSysMem = vertices;

		HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pVertexBuffer);
		if (FAILED(hr)) return 30;

		return 0;
	}
	case Mesh::Mirror:
		break;
	default:
		break;
	}

	
	
}

int Mesh::initIndexBuffer(ID3D11Device* pD3DDevice)
{
	switch (_object)
	{
	case Mesh::Cube:
	{


		_indexCount = 36;

		WORD indices[] =
		{
			//F
			0, 1, 2,
			1, 3, 2,
			//R
			4,5,6,
			5,7,6,
			//L
			8,9,10,
			9,11,10,
			//B
			12,13,14,
			13,15,14,
			//T
			16,17,18,
			17,19,18,
			//Bot
			20,21,22,
			21,23,22
		};

		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = _indexCount * sizeof(WORD);
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA initialData = {};
		initialData.pSysMem = indices;

		HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pIndexBuffer);
		if (FAILED(hr)) return 35;

		return 0;
	}
	case Mesh::Floor:
	{
		_indexCount = 6;

		WORD indices[] =
		{

			0, 1, 2,
			1, 3, 2
		};

		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = _indexCount * sizeof(WORD);
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA initialData = {};
		initialData.pSysMem = indices;

		HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pIndexBuffer);
		if (FAILED(hr)) return 35;

		return 0;
	}
	case Mesh::Wall:
	{
		_indexCount = 6;

		WORD indices[] =
		{

			0, 1, 2,
			1, 3, 2
		};

		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = _indexCount * sizeof(WORD);
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA initialData = {};
		initialData.pSysMem = indices;

		HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pIndexBuffer);
		if (FAILED(hr)) return 35;

		return 0;
	}
	case Mesh::Mirror:
		break;
	default:
		break;
	}
	
}
