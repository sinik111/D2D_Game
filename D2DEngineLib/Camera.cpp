#include "pch.h"
#include "Camera.h"

#include "Transform.h"
#include "Screen.h"

Camera* Camera::s_mainCamera = nullptr;

Camera::Camera()
{
	s_mainCamera = this;
}

Camera::~Camera()
{
	s_mainCamera = nullptr;
}

void Camera::Update()
{
	if (m_isDirty || GetTransform()->GetIsDirtyThisFrame())
	{
		Matrix3x2 worldMatrix = GetTransform()->GetWorldMatrix();

		Vector2 scale = worldMatrix.GetScale();

		assert((std::abs(scale.x) > 0.0f && std::abs(scale.y) > 0.0f) && "0으로 나누고 있습니다");

		scale.x = 1 / scale.x * m_zoomFactor;
		scale.y = 1 / scale.y * m_zoomFactor;

		worldMatrix = Matrix3x2::Scale(scale) * worldMatrix;

		m_cachedViewMatrix = worldMatrix.Inverse();
		
	worldMatrix.ResetScale(m_zoomFactor, m_zoomFactor);

		m_isDirty = false;
	}
}

const Matrix3x2& Camera::GetViewMatrix()
{
	return m_cachedViewMatrix;
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

	m_isDirty = true;
}
