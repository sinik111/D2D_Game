#pragma once

#include "Component.h"
#include "Matrix3x2.h"

class Camera :
	public Component
{
public:
	static Camera* s_mainCamera; // ���� �ʿ�, CameraManager ���

private:
	Matrix3x2 m_cachedViewMatrix;
	float m_zoomFactor = 1.0f;
	bool m_isDirty = true;

public:
	Camera();
	~Camera() override;

public:
	void Update();;
	const Matrix3x2& GetViewMatrix();

	float GetZoom() const;
	void SetZoom(float zoomFactor);
};