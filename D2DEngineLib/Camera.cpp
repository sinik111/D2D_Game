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
	if (m_isDirty || GetTransform()->IsDirtyThisFrame())
	{
		Matrix3x2 worldMatrix = GetTransform()->GetWorldMatrix();

		Vector2 scale = worldMatrix.GetScale();

		assert((std::abs(scale.x) > 0.0f && std::abs(scale.y) > 0.0f) && "0으로 나누고 있습니다");

		scale.x = 1 / scale.x * m_zoomFactor;
		scale.y = 1 / scale.y * m_zoomFactor;

		worldMatrix = Matrix3x2::Scale(scale) * worldMatrix;

		m_cachedViewMatrix = worldMatrix.Inverse();

		float halfScreenWidth = Screen::Get().GetWidth() * 0.5f;
		float halfScreenHeight = Screen::Get().GetHeight() * 0.5f;

		D2D1_RECT_F screenRect{
			-halfScreenWidth,
			-halfScreenHeight,
			halfScreenWidth,
			halfScreenHeight
		};

		m_visibleBounds = Bounds::RectToWorldBounds(screenRect, worldMatrix);

		m_isDirty = false;
	}
}

const Bounds& Camera::GetVisibleBounds() const
{
	return m_visibleBounds;
}

const Matrix3x2& Camera::GetViewMatrix() const
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
