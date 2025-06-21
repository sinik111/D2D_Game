#pragma once

#include "Component.h"
#include "Matrix3x2.h"

class Camera :
	public Component
{
public:
	static Camera* s_mainCamera; // ���� �ʿ�, CameraManager ���

private:
	float m_zoomFactor = 1.0f;

public:
	Camera();
	~Camera() override;

public:
	Matrix3x2 GetViewMatrix();

	float GetZoom();
	void SetZoom(float zoomFactor);
};