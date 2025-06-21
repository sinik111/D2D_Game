#pragma once

#include "Component.h"
#include "Matrix3x2.h"

class Camera :
	public Component
{
public:
	static Camera* s_mainCamera; // 수정 필요, CameraManager 등등

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