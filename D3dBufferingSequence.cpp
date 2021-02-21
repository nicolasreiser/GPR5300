#include "D3dBufferingSequence.h"

int D3dBufferingSequence::Init(ID3D11Device* pD3DDevice)
{
	int error = createUpdatedBuffer(pD3DDevice);
	if (error != 0) return error;
}


int D3dBufferingSequence::createUpdatedBuffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(UpdatedBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, nullptr, &_UpdatedBuffer);
	if (FAILED(hr)) return 46;

	return 0;
}

void D3dBufferingSequence::setUpdatedBuffer(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* world, XMFLOAT4X4* view, XMFLOAT4X4* projection)
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(_UpdatedBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	XMMATRIX worldMatrix = XMLoadFloat4x4(world);
	XMMATRIX viewMatrix = XMLoadFloat4x4(view);
	XMMATRIX projectionMatrix = XMLoadFloat4x4(projection);

	XMMATRIX wvpMatrix = worldMatrix * viewMatrix * projectionMatrix;
	wvpMatrix = XMMatrixTranspose(wvpMatrix);
	worldMatrix = XMMatrixTranspose(worldMatrix);

	UpdatedBuffer* pBuffer = reinterpret_cast<UpdatedBuffer*>(data.pData);
	XMStoreFloat4x4(&pBuffer->worldViewProjectionMatrix, wvpMatrix);
	XMStoreFloat4x4(&pBuffer->worldMatrix, worldMatrix);


	pD3DDeviceContext->Unmap(_UpdatedBuffer, 0);

	pD3DDeviceContext->VSSetConstantBuffers(1, 1, &_UpdatedBuffer);
}
