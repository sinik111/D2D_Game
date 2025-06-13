#include "pch.h"
#include "Transform.h"

#include "MyMath.h"

Transform::Transform()
	: m_parent{ nullptr }, m_position{ 0.0f, 0.0f },
	m_rotation{ 0.0f }, m_scale{ 1.0f, 1.0f },
	m_isDirty{ true }
{
}

Vector2 Transform::GetPosition() const
{
	return m_position;
}

float Transform::GetRotation() const
{
	return m_rotation;
}

Vector2 Transform::GetScale() const
{
	return m_scale;
}

Matrix3x2 Transform::GetWorldMatrix() const
{
	if (m_parent != nullptr)
	{
		return Matrix3x2::Scale(m_scale) *
			Matrix3x2::Rotation(m_rotation) *
			Matrix3x2::Translation(m_position) *
			m_parent->GetWorldMatrix();
	}

	return Matrix3x2::Scale(m_scale) *
		Matrix3x2::Rotation(m_rotation) *
		Matrix3x2::Translation(m_position);
}

Transform* Transform::GetParent() const
{
	return m_parent;
}

void Transform::SetPosition(float x, float y)
{
	m_position = Vector2(x, y);
}

void Transform::SetPosition(const Vector2& position)
{
	m_position = position;
}

void Transform::SetRotation(float angle)
{
	m_rotation = angle;
}

void Transform::SetScale(float x, float y)
{
	m_scale = Vector2(x, y);
}

void Transform::SetScale(const Vector2& scale)
{
	m_scale = scale;
}

void Transform::SetParent(Transform* parent)
{
	m_parent = parent;
}

void Transform::Reset()
{
	SetPosition(0.0f, 0.0f);
	SetRotation(0.0f);
	SetScale(1.0f, 1.0f);
}

void Transform::Translate(float x, float y)
{
	m_position += Vector2(x, y);
}

void Transform::Translate(const Vector2& movement)
{
	m_position += movement;
}

void Transform::Rotate(float angle)
{
	m_rotation += angle;
	
	if (m_rotation > 360.0f)
	{
		m_rotation -= 360.0f;
	}
}
