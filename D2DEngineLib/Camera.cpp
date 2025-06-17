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
	return GetTransform()->GetWorldMatrix().Inverse();
}