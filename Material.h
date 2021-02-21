#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Light.h"
#include"Camera.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

struct ShaderParameters
{
	//TMP values
	XMFLOAT2 Tiling;
	XMFLOAT2 Offset;
	XMFLOAT2 SpecularPower;
	XMFLOAT2 Glossiness;
	LPCWSTR VertexShaderName;
	LPCWSTR PixelShaderName;

};

class Material
{
public:
	
	int init(ID3D11Device* pD3DDevice, LPCWSTR textureName, ShaderParameters* parameters);
	void render(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix, Camera* camera);
	void deInit();

	void setLight(ID3D11DeviceContext* pD3DDeviceContext, Light& lightData);

	// parameter variables
	
private:
	// helper methods
	int createVertexShader(ID3D11Device* pD3DDevice);
	int createPixelShader(ID3D11Device* pD3DDevice);
	int createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pBlob);
	int createMatetialConstantBuffer(ID3D11Device* pD3DDevice);
	int createUpdatedBuffer(ID3D11Device* pD3DDevice);
	void setMaterialConstantBuffer(ID3D11DeviceContext* pD3DDeviceContext);
	void setUpdatedBuffer(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix, Camera* camera);
	int createTextureAndSampler(ID3D11Device* pD3DDevice, LPCWSTR textureName);
	int createPixelShaderBuffer(ID3D11Device* pD3DDevice);

	// shaders
	ID3D11VertexShader* _pVertexShader = nullptr;
	ID3D11PixelShader* _pPixelShader = nullptr;

	// input layout
	ID3D11InputLayout* _pInputLayout = nullptr;

	// textures and samplers
	ID3D11ShaderResourceView* _pMainTexture = nullptr;
	ID3D11SamplerState* _pMainSampler = nullptr;

	// constant buffers
	ID3D11Buffer* _pMaterialConstantBuffer = nullptr;
	struct MaterialConstantBuffer
	{
		XMFLOAT4 tilingOffset;
		XMFLOAT4 SpecularPowerGlossiness;
	};

	ID3D11Buffer* _UpdatedBuffer = nullptr;
	struct UpdatedBuffer
	{
		XMFLOAT4X4 worldViewProjectionMatrix;
		XMFLOAT4X4 worldMatrix;
		XMFLOAT3 WorldSpaceCameraPos;
		FLOAT Time;
	};

	ID3D11Buffer* _pPixelShaderBuffer = nullptr;
	struct PixelShaderBuffer
	{
		Light lightData;
	};

	ShaderParameters* _parameters;
};

