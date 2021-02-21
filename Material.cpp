#include "Material.h"
#include "WICTextureLoader11.h"
#include <d3dcompiler.h>
#include "Utilities.h"
#include<string>

int Material::init(ID3D11Device* pD3DDevice, LPCWSTR textureName, ShaderParameters* parameters)
{
	//_material.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	//_material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	//_material.Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	//_material.Emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
	//_material.Power = 2048.0f;

	_parameters = parameters;
	int error = createVertexShader(pD3DDevice);
	if (error != 0) return error;

	error = createPixelShader(pD3DDevice);
	if (error != 0) return error;

	error = createMatetialConstantBuffer(pD3DDevice);
	if (error != 0) return error;

	error = createUpdatedBuffer(pD3DDevice);
	if (error != 0) return error;

	error = createTextureAndSampler(pD3DDevice, textureName);
	if (error != 0) return error;

	error = createPixelShaderBuffer(pD3DDevice);
	if (error != 0) return error;

	return 0;
}

void Material::render(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix, Camera* camera)
{
	pD3DDeviceContext->IASetInputLayout(_pInputLayout);
	pD3DDeviceContext->VSSetShader(_pVertexShader, nullptr, 0);
	pD3DDeviceContext->PSSetShader(_pPixelShader, nullptr, 0);

	setMaterialConstantBuffer(pD3DDeviceContext);
	setUpdatedBuffer(pD3DDeviceContext, worldMatrix,camera);

	pD3DDeviceContext->PSSetShaderResources(0, 1, &_pMainTexture);
	pD3DDeviceContext->PSSetSamplers(0, 1, &_pMainSampler);
}

void Material::deInit()
{
	safeRelease<ID3D11ShaderResourceView>(_pMainTexture);
	safeRelease<ID3D11SamplerState>(_pMainSampler);
	safeRelease<ID3D11Buffer>(_pMaterialConstantBuffer);
	safeRelease<ID3D11VertexShader>(_pVertexShader);
	safeRelease<ID3D11PixelShader>(_pPixelShader);
	safeRelease<ID3D11InputLayout>(_pInputLayout);
}

void Material::setLight(ID3D11DeviceContext* pD3DDeviceContext, Light& lightData)
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(_pPixelShaderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	PixelShaderBuffer* pBuffer = reinterpret_cast<PixelShaderBuffer*>(data.pData);
	pBuffer->lightData = lightData;

	pD3DDeviceContext->Unmap(_pPixelShaderBuffer, 0);

	pD3DDeviceContext->PSSetConstantBuffers(0, 1, &_pPixelShaderBuffer);
}

int Material::createVertexShader(ID3D11Device* pD3DDevice)
{
	ID3DBlob* pCompiledCode = nullptr;
	HRESULT hr = D3DCompileFromFile(
		_parameters->VertexShaderName, // shader filename
		nullptr, nullptr, // optional macros & includes
		"main", // entry point function name
		"vs_4_0", // shader type & version
		0, 0, // optional flags
		&pCompiledCode, // compiled code target
		nullptr // optional blob for all compile errors
	);
	if (FAILED(hr)) return 40;

	hr = pD3DDevice->CreateVertexShader(pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &_pVertexShader);
	if (FAILED(hr)) return 42;

	int error = createInputLayout(pD3DDevice, pCompiledCode);
	if (error != 0) return error;

	safeRelease<ID3DBlob>(pCompiledCode);

	return 0;
}

int Material::createPixelShader(ID3D11Device* pD3DDevice)
{
	ID3DBlob* pCompiledCode = nullptr;
	HRESULT hr = D3DCompileFromFile(
		_parameters->PixelShaderName, // shader filename
		nullptr, nullptr, // optional macros & includes
		"main", // entry point function name
		"ps_4_0", // shader type & version
		0, 0, // optional flags
		&pCompiledCode, // compiled code target
		nullptr // optional blob for all compile errors
	);
	if (FAILED(hr)) return 46;

	hr = pD3DDevice->CreatePixelShader(pCompiledCode->GetBufferPointer(), pCompiledCode->GetBufferSize(), nullptr, &_pPixelShader);
	if (FAILED(hr)) return 47;

	safeRelease<ID3DBlob>(pCompiledCode);

	return 0;
}

int Material::createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pBlob)
{
	D3D11_INPUT_ELEMENT_DESC elements[5] = {};

	// position
	elements[0].SemanticName = "POSITION";
	elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	// color
	elements[1].SemanticName = "COLOR";
	elements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// normal
	elements[2].SemanticName = "NORMAL";
	elements[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// uv
	elements[3].SemanticName = "TEXCOORD";
	elements[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	elements[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	elements[4].SemanticName = "TEXCOORD";
	elements[4].SemanticIndex = 1;
	elements[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;


	HRESULT hr = pD3DDevice->CreateInputLayout(elements, 5, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &_pInputLayout);
	OutputDebugString(std::to_wstring((long)hr).c_str());
	if (FAILED(hr)) return 44;

	return 0;
}

int Material::createMatetialConstantBuffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(MaterialConstantBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, nullptr, &_pMaterialConstantBuffer);
	if (FAILED(hr)) return 46;

	return 0;
}



void Material::setMaterialConstantBuffer(ID3D11DeviceContext* pD3DDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(_pMaterialConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	XMVECTOR tilingOffset = { _parameters->Tiling.x, _parameters->Tiling.y,_parameters->Offset.x,_parameters->Offset.y };
	XMVECTOR SpecularPowerGlossiness = { _parameters->SpecularPower.x,_parameters->Glossiness.x,0,0 };
	
	MaterialConstantBuffer* pBuffer = reinterpret_cast<MaterialConstantBuffer*>(data.pData);

	XMStoreFloat4(&pBuffer->tilingOffset, tilingOffset);
	XMStoreFloat4(&pBuffer->SpecularPowerGlossiness, SpecularPowerGlossiness);


	pD3DDeviceContext->Unmap(_pMaterialConstantBuffer, 0);
	pD3DDeviceContext->PSSetConstantBuffers(1, 1, &_pMaterialConstantBuffer);
}

void Material::setUpdatedBuffer(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* world, Camera* camera)
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(_UpdatedBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	XMMATRIX worldMatrix = XMLoadFloat4x4(world);
	XMMATRIX viewMatrix =camera->getViewMatrix();
	XMMATRIX projectionMatrix = camera->getProjectionMatrix();

	XMMATRIX wvpMatrix = worldMatrix * viewMatrix * projectionMatrix;
	wvpMatrix = XMMatrixTranspose(wvpMatrix);
	worldMatrix = XMMatrixTranspose(worldMatrix);

	UpdatedBuffer* pBuffer = reinterpret_cast<UpdatedBuffer*>(data.pData);
	XMStoreFloat4x4(&pBuffer->worldViewProjectionMatrix, wvpMatrix);
	XMStoreFloat4x4(&pBuffer->worldMatrix, worldMatrix);

	pBuffer->WorldSpaceCameraPos = camera->GetPositionFloat3();

	pD3DDeviceContext->Unmap(_UpdatedBuffer, 0);

	pD3DDeviceContext->VSSetConstantBuffers(1, 1, &_UpdatedBuffer);
}

int Material::createTextureAndSampler(ID3D11Device* pD3DDevice, LPCWSTR textureName)
{
	HRESULT hr = CreateWICTextureFromFile(pD3DDevice, textureName, nullptr, &_pMainTexture);
	if (FAILED(hr)) return 48;

	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	hr = pD3DDevice->CreateSamplerState(&desc, &_pMainSampler);
	if (FAILED(hr)) return 49;

	return 0;
}

int Material::createUpdatedBuffer(ID3D11Device* pD3DDevice)
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
int Material::createPixelShaderBuffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(PixelShaderBuffer);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, nullptr, &_pPixelShaderBuffer);
	if (FAILED(hr)) return 47;

	return 0;
}
