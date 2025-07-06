#include "pch.h"
#include "Camera.h"

#include "Transform.h"

Camera* Camera::s_mainCamera = nullptr;

Camera::Camera()
{
	s_mainCamera = this;
}

Camera::~Camera()
{
	s_mainCamera = nullptr;
}

Matrix3x2 Camera::GetViewMatrix()
{
	// caching, dirty flag 적용 필요
	Matrix3x2 worldMatrix = GetTransform()->GetWorldMatrix();



	worldMatrix.ResetScale(m_zoomFactor, m_zoomFactor);

	return worldMatrix.Inverse();
}

float Camera::GetZoom() const
{
	return m_zoomFactor;
}

void Camera::SetZoom(float zoomFactor)
{
	m_zoomFactor = zoomFactor;

	if (m_zoomFactor < 0.01f)
	{
		m_zoomFactor = 0.01f;
	}
}
