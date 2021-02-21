#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Time.h"

using namespace DirectX;

class Camera
{
public:
	int init(INT screenWidth, INT screenHeight);
	void deInit();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
	void Update();
	
	const XMMATRIX& getViewMatrix() const;
	 XMFLOAT4X4* getViewMatrix4X4();
	const XMMATRIX& getProjectionMatrix()const;
	 XMFLOAT4X4* getProjectionMatrix4X4();

	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

	void SetPosition(const XMVECTOR& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const XMVECTOR& pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const XMVECTOR& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR& rot);
	void AdjustRotation(float x, float y, float z);
	void SetTime(Time* time);
	void SetDeltaMousePosition();
	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetLeftVector();
	const XMVECTOR& GetRightVector();



private:

	void UpdateViewMatrix();

	XMMATRIX _viewMatrix;
	XMFLOAT4X4 _viewMatrix4x4;
	XMMATRIX _projectionMatrix;
	XMFLOAT4X4 _projectionMatrix4x4;
	XMVECTOR _posVector;
	XMVECTOR _rotVector;
	XMFLOAT3 _pos;
	XMFLOAT3 _rot;
	Time *_time;
	POINT _mousePosition;
	POINT _mousePositionOld;
	POINT _mousePositionDelta;
	
	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR _vecForward;
	XMVECTOR _vecLeft;
	XMVECTOR _vecRight;
	XMVECTOR _vecBackward;
};

