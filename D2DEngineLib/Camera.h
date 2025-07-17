#pragma once

#include "Component.h"

class Camera :
	public Component
{
public:
	static Camera* s_mainCamera; // 여러 카메라 사용 시 수정 필요

private:
	Matrix3x2 m_cachedViewMatrix;
	Bounds m_visibleBounds;
	float m_zoomFactor = 1.0f;
	bool m_isDirty = true;

public:
	Camera();
	~Camera() override;

public:
	void Update();
	const Matrix3x2& GetViewMatrix() const;
	const Bounds& GetVisibleBounds() const;

	float GetZoom() const;
	void SetZoom(float zoomFactor);
};