#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Material.h"
#include "Light.h"
#include "Mesh.h"


class GameObject
{

public:
	int init(Material* material, Mesh* mesh);
	void update(float deltaTime);
	void render(ID3D11DeviceContext* pD3DDeviceContext,Camera* camera);
	void deInit();

	void setPosition(float X, float Y, float Z);
	void setRotation(float X, float Y, float Z);
	void SetScale(float X, float Y, float Z);
	void AdjustPosition(float X, float Y, float Z);
	void AdjustRotation(float X, float Y, float Z);
	void AdjustScale(float X, float Y, float Z);

	XMFLOAT4X4* getWorldMatrix() { return &_worldMatrix; }

private:
	XMFLOAT4X4 _worldMatrix = {};
	Material* _material;
	Mesh* _mesh;
	float posX;
	float posY;
	float posZ;
	float rotX;
	float rotY;
	float rotZ;
	float scaleX = 1;
	float scaleY = 1;
	float scaleZ = 1;
};

