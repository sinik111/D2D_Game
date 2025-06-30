#include "pch.h"
#include "Transform.h"

#include "MyMath.h"
#include "ContainerUtility.h"
#include "ComponentSystem.h"

static constexpr float MaxDegree = 360.0f;

Transform::~Transform()
{
	if (m_parent != nullptr)
	{
		m_parent->RemoveChild(this);
	}

	for (const auto& child : m_children)
	{
		child->SetParent(nullptr);
	}
}

const Vector2& Transform::GetLocalPosition() const
{
	return m_localPosition;
}

float Transform::GetLocalRotation() const
{
	return m_localRotation;
}

const Vector2& Transform::GetLocalScale() const
{
	return m_localScale;
}

const Vector2& Transform::GetWorldScale()
{
	if (m_isDirty || m_isScaleDirty)
	{
		m_worldScale = m_parent != nullptr ?
			Vector2(m_localScale.GetX() * m_parent->GetWorldScale().GetX(),
				m_localScale.GetY() * m_parent->GetWorldScale().GetY()) :
			m_localScale;

		m_isScaleDirty = false;
	}

	return m_worldScale;
}

const Vector2& Transform::GetWorldPosition()
{
	if (m_isDirty || m_isPositionDirty)
	{
		m_worldPosition = m_parent != nullptr ?
			GetWorldMatrix().GetPosition() :
			m_localPosition;

		m_isPositionDirty = false;
	}

	return m_worldPosition;
}

const Matrix3x2& Transform::GetWorldMatrix()
{
	if (m_isDirty)
	{
		Matrix3x2 localMatrix = MakeLocalMatrix();

		m_cachedWorld = m_parent != nullptr ? 
			localMatrix * m_parent->GetWorldMatrix() : 
			localMatrix;

		m_isDirty = false;
	}

	return m_cachedWorld;
}

Transform* Transform::GetParent() const
{
	return m_parent;
}

const std::vector<Transform*>& Transform::GetChildren()
{
	return m_children;
}

void Transform::SetLocalPosition(float x, float y)
{
	SetLocalPosition(Vector2(x, y));
}

void Transform::SetLocalPosition(const Vector2& position)
{
	m_localPosition = position;

	MarkDirty();
	MarkPositionDirty();
}

void Transform::SetLocalRotation(float angle)
{
	m_localRotation = angle;

	MarkDirty();
}

void Transform::SetLocalScale(float x, float y)
{
	SetLocalScale(Vector2(x, y));
}

void Transform::SetLocalScale(const Vector2& scale)
{
	m_localScale = scale;

	MarkScaleDirty();
	MarkDirty();
}

void Transform::SetParent(Transform* parent)
{
	if (m_parent == parent)
	{
		return;
	}

	if (m_parent != nullptr)
	{
		m_parent->RemoveChild(this);
	}

	m_parent = parent;

	if (m_parent != nullptr)
	{
		m_parent->AddChild(this);
	}

	MarkDirty();
	MarkScaleDirty();
	MarkPositionDirty();
}

void Transform::Reset()
{
	SetLocalPosition(0.0f, 0.0f);
	SetLocalRotation(0.0f);
	SetLocalScale(1.0f, 1.0f);
}

void Transform::Translate(float x, float y)
{
	Translate(Vector2(x, y));
}

void Transform::Translate(const Vector2& movement)
{
	SetLocalPosition(m_localPosition + movement);
}

void Transform::Rotate(float angle)
{
	m_localRotation += angle;

	if (m_localRotation > MaxDegree)
	{
		m_localRotation -= MaxDegree;
	}
	else if (m_localRotation < 0.0f)
	{
		m_localRotation += MaxDegree;
	}

	MarkDirty();
}

Matrix3x2 Transform::MakeLocalMatrix()
{
	return Matrix3x2::Scale(m_localScale) * 
		Matrix3x2::Rotation(m_localRotation) * 
		Matrix3x2::Translation(m_localPosition);
}

void Transform::MarkDirty()
{
	if (!m_isDirty)
	{
		m_isDirty = true;

		for (const auto& child : m_children)
		{
			child->MarkDirty();
		}
	}
}

void Transform::MarkScaleDirty()
{
	if (!m_isScaleDirty)
	{
		m_isScaleDirty = true;

		for (const auto& child : m_children)
		{
			child->MarkScaleDirty();
		}
	}
}

void Transform::MarkPositionDirty()
{
	if (!m_isPositionDirty)
	{
		m_isPositionDirty = true;

		for (const auto& child : m_children)
		{
			child->MarkPositionDirty();
		}
	}
}

void Transform::AddChild(Transform* child)
{
	m_children.push_back(child);
}

void Transform::RemoveChild(Transform* child)
{
	Util::StableErase(m_children, child);
}