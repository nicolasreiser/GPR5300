#include "GameObject.h"
#include "D3D11.h"

int GameObject::init(Material* material, Mesh* mesh)
{
	XMStoreFloat4x4(&_worldMatrix, XMMatrixIdentity());
	_material = material;
	_mesh = mesh;
	return 0;
}

void GameObject::update(float deltaTime)
{
	XMMATRIX translation = XMMatrixTranslation(posX, posY, posZ);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(rotX, rotY, rotZ);
	XMMATRIX scale = XMMatrixScaling(scaleX, scaleY, scaleZ);

	XMStoreFloat4x4(&_worldMatrix, scale * rotation * translation);
}

void GameObject::render(ID3D11DeviceContext* pD3DDeviceContext, Camera* camera)
{
	_material->render(pD3DDeviceContext, &_worldMatrix, camera);
	_mesh->render(pD3DDeviceContext);
}

void GameObject::deInit()
{
	//TODO deinitiallize GO
}

void GameObject::setPosition(float X, float Y, float Z)
{
	posX = X;
	posY = Y;
	posZ = Z;
}

void GameObject::setRotation(float X, float Y, float Z)
{
	rotX = X;
	rotY = Y;
	rotZ = Z;
}

void GameObject::SetScale(float X, float Y, float Z)
{
	scaleX = X;
	scaleY = Y;
	scaleZ = Z;
}

void GameObject::AdjustRotation(float X, float Y, float Z)
{
	rotX += X;
	rotY += Y;
	rotZ += Z;
}

void GameObject::AdjustScale(float X, float Y, float Z)
{
	scaleX += X;
	scaleY += Y;
	scaleZ += Z;
}

void GameObject::AdjustPosition(float X, float Y, float Z)
{
	posX += X;
	posY += Y;
	posZ += Z;
}


