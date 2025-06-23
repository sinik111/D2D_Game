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

const Vector2& Transform::GetPosition() const
{
	return m_position;
}

float Transform::GetRotation() const
{
	return m_rotation;
}

const Vector2& Transform::GetScale() const
{
	return m_scale;
}

const Matrix3x2& Transform::GetLocalMatrix()
{
	if (m_isLocalDirty)
	{
		m_cachedLocal = Matrix3x2::Scale(m_scale) * Matrix3x2::Rotation(m_rotation) * Matrix3x2::Translation(m_position);

		m_isLocalDirty = false;
	}

	return m_cachedLocal;
}

const Matrix3x2& Transform::GetWorldMatrix()
{
	if (m_isWorldDirty || m_isLocalDirty)
	{
		m_cachedWorld = m_parent != nullptr ? GetLocalMatrix() * m_parent->GetWorldMatrix() : GetLocalMatrix();

		m_isWorldDirty = false;
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

void Transform::SetPosition(float x, float y)
{
	m_position = Vector2(x, y);

	m_isLocalDirty = true;
	MarkWorldDirty();
}

void Transform::SetPosition(const Vector2& position)
{
	m_position = position;

	m_isLocalDirty = true;
	MarkWorldDirty();
}

void Transform::SetRotation(float angle)
{
	m_rotation = angle;

	m_isLocalDirty = true;
	MarkWorldDirty();
}

void Transform::SetScale(float x, float y)
{
	m_scale = Vector2(x, y);

	m_isLocalDirty = true;
	MarkWorldDirty();
}

void Transform::SetScale(const Vector2& scale)
{
	m_scale = scale;

	m_isLocalDirty = true;
	MarkWorldDirty();
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

	MarkWorldDirty();
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

	m_isLocalDirty = true;
	MarkWorldDirty();
}

void Transform::Translate(const Vector2& movement)
{
	m_position += movement;

	m_isLocalDirty = true;
	MarkWorldDirty();
}

void Transform::Rotate(float angle)
{
	m_rotation += angle;

	if (m_rotation > MaxDegree)
	{
		m_rotation -= MaxDegree;
	}
	else if (m_rotation < 0.0f)
	{
		m_rotation += MaxDegree;
	}

	m_isLocalDirty = true;
	MarkWorldDirty();
}

void Transform::MarkWorldDirty()
{
	if (!m_isWorldDirty)
	{
		m_isWorldDirty = true;

		for (const auto& child : m_children)
		{
			child->MarkWorldDirty();
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
