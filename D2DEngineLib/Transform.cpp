#include "pch.h"
#include "Transform.h"

#include "MyMath.h"

Transform::Transform()
	: m_parent{ nullptr }, m_position{ 0.0f, 0.0f },
	m_rotation{ 0.0f }, m_scale{ 1.0f, 1.0f },
	m_isDirty{ true }
{
}

D2D1_VECTOR_2F Transform::GetPosition() const
{
	return m_position;
}

float Transform::GetRotation() const
{
	return m_rotation;
}

D2D1_VECTOR_2F Transform::GetScale() const
{
	return m_scale;
}

D2D1::Matrix3x2F Transform::GetMatrix() const
{
	if (m_parent != nullptr)
	{
		return D2D1::Matrix3x2F::Scale(m_scale.x, m_scale.y) *
			D2D1::Matrix3x2F::Rotation(m_rotation) *
			D2D1::Matrix3x2F::Translation(m_position.x, m_position.y) *
			m_parent->GetMatrix();
	}

	return D2D1::Matrix3x2F::Scale(m_scale.x, m_scale.y) *
		D2D1::Matrix3x2F::Rotation(m_rotation) *
		D2D1::Matrix3x2F::Translation(m_position.x, m_position.y);
}

Transform* Transform::GetParent() const
{
	return m_parent;
}

void Transform::SetPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}

void Transform::SetPosition(D2D1_VECTOR_2F position)
{
	m_position = position;
}

void Transform::SetRotation(float angle)
{
	m_rotation = angle;
}

void Transform::SetScale(float x, float y)
{
	m_scale.x = x;
	m_scale.y = y;
}

void Transform::SetScale(D2D1_VECTOR_2F scale)
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
	m_position.x += x;
	m_position.y += y;
}

void Transform::Translate(D2D1_VECTOR_2F movement)
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
