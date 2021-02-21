#include "Camera.h"
#include <DirectXMath.h>
#include"Time.h"



using namespace DirectX;

int Camera::init(INT screenWidth, INT screenHeight)
{
	//// view matrix
	//XMMATRIX viewMatrix = XMMatrixLookToLH(
	//	XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), // camera position
	//	XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), // camera forward direction
	//	XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) // camera up direction
	//);
	//XMStoreFloat4x4(&_viewMatrix, viewMatrix);

	//// projection Matrix - perspective
	//XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(
	//	XM_PI * 0.3333333f, // field of view in radians
	//	static_cast<FLOAT>(screenWidth) / static_cast<FLOAT>(screenHeight), // aspect ratio
	//	0.3f, 1000.0f // near & far clipping
	//);
	//XMStoreFloat4x4(&_projectionMatrix, projectionMatrix);

	//TMP
	_pos = XMFLOAT3(0.0f, 0.0f,0.0f);
	_posVector = XMLoadFloat3(&_pos);
	_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_rotVector = XMLoadFloat3(&_rot);
	SetProjectionValues(90.0f, screenWidth / screenHeight, 0.3f, 1000.0f);
	UpdateViewMatrix();
	GetCursorPos(&_mousePositionOld);
	return 0;
}

void Camera::deInit()
{
	//TODO Deinitialize

}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	_projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
	XMStoreFloat4x4(&_projectionMatrix4x4,_projectionMatrix);
}

void Camera::Update()
{
	//MouseEvent me 
	//if(MouseEvent::EventType::RAW_MOVE)
	GetCursorPos(&_mousePosition);
	SetDeltaMousePosition();
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		AdjustRotation(1 *_mousePositionDelta.y * (_time->getDeltaTime()+0.002f) ,1 *_mousePositionDelta.x * (_time->getDeltaTime() + 0.002f),0);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		AdjustPosition(0, 1 * _time->getDeltaTime(), 0);
	}
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		AdjustPosition(0, -1 * _time->getDeltaTime(), 0);
	}if (GetAsyncKeyState(0x57) & 0x8000) // W
	{
		AdjustPosition( 0,0,1 * _time->getDeltaTime());
	}if (GetAsyncKeyState(0x41) & 0x8000) // A
	{
		AdjustPosition( -1 * _time->getDeltaTime(),0, 0);
	}if (GetAsyncKeyState(0x53) & 0x8000) // S
	{
		AdjustPosition(0, 0, -1 * _time->getDeltaTime());
	}
	if (GetAsyncKeyState(0x44) & 0x8000) // D
	{
		AdjustPosition(1 * _time->getDeltaTime(), 0, 0);
		
	}
	if (GetAsyncKeyState(0x51) & 0x8000) // Q
	{
		AdjustRotation(0,-1 * _time->getDeltaTime(), 0);

	}
	
}

const XMMATRIX& Camera::getViewMatrix() const
{
	return _viewMatrix;
}


 XMFLOAT4X4* Camera::getViewMatrix4X4() 
{
	return &_viewMatrix4x4;
}

const XMMATRIX& Camera::getProjectionMatrix() const
{
	return _projectionMatrix;
}

XMFLOAT4X4* Camera::getProjectionMatrix4X4() 
{
	return &_projectionMatrix4x4;
}

const XMVECTOR& Camera::GetPositionVector() const
{
	return _posVector;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
	return _pos;
}

const XMVECTOR& Camera::GetRotationVector() const
{
	return _rotVector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
	return _rot;
}

void Camera::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&_pos, pos);
	_posVector = pos;
	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	_pos = XMFLOAT3(x, y, z);
	_posVector = XMLoadFloat3(&_pos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& pos)
{
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(_rot.x, _rot.y, _rot.z);
	
	XMVECTOR v = XMVector3Transform(pos, camRotationMatrix);
	XMFLOAT3 v2;
	XMStoreFloat3(&v2, v);

	_pos.x += v2.x;
	_pos.y += v2.y;
	_pos.z += v2.z;
	_posVector = XMLoadFloat3(&_pos);

	UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	XMVECTOR MovementAmmount = { x,y,z };
	AdjustPosition(MovementAmmount);
}

void Camera::SetRotation(const XMVECTOR& rot)
{
	_rotVector = rot;
	XMStoreFloat3(&_rot, rot);
	UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	_rot = XMFLOAT3(x, y, z);
	_rotVector = XMLoadFloat3(&_rot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& rot)
{
	_rotVector += rot;
	XMStoreFloat3(&_rot, rot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	_rot.x += x;
	_rot.y += y;
	_rot.z += z;
	_rotVector = XMLoadFloat3(&_rot);
	UpdateViewMatrix();
}

void Camera::SetTime(Time* time)
{
	_time = time;
}


void Camera::SetDeltaMousePosition()
{
	_mousePositionDelta = { _mousePosition.x - _mousePositionOld.x, _mousePosition.y - _mousePositionOld.y };
	_mousePositionOld = _mousePosition;
}

const XMVECTOR& Camera::GetForwardVector()
{
	return _vecForward;
}

const XMVECTOR& Camera::GetBackwardVector()
{
	return _vecBackward;
}

const XMVECTOR& Camera::GetLeftVector()
{
	return _vecLeft;
}

const XMVECTOR& Camera::GetRightVector()
{
	return _vecRight;
}

// Updates the matrix and the movement Vectors
void Camera::UpdateViewMatrix()
{
	//camera rotation matrix
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(_rot.x, _rot.y, _rot.z);
	//calculate unit vector of cam target
	XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	// Adjust cam target to be offset by the cameras pos
	camTarget += _posVector;
	// Calculate direction based on rotation
	XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotationMatrix);
	//rebuild matrix
	_viewMatrix = XMMatrixLookAtLH(_posVector, camTarget, upDir);

	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(_rot.x, _rot.y, _rot.z);
	_vecForward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	_vecBackward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	_vecLeft = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	_vecRight = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
	XMStoreFloat4x4(&_viewMatrix4x4, _viewMatrix);
}

