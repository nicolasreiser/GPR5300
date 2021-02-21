#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class D3dBufferingSequence
{
public:
	int Init(ID3D11Device* pD3DDevice);

	int createUpdatedBuffer(ID3D11Device* pD3DDevice);
	void setUpdatedBuffer(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix, XMFLOAT4X4* viewMatrix, XMFLOAT4X4* projectionMatrix);

private:

	ID3D11Buffer* _UpdatedBuffer = nullptr;
	struct UpdatedBuffer
	{
		XMFLOAT4X4 worldViewProjectionMatrix;
		XMFLOAT4X4 worldMatrix;
		XMFLOAT3 WorldSpaceCameraPos;
		FLOAT Time;
	};

};

