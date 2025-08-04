#include "pch.h"
#include "BoxCollider2D.h"

#include "Transform.h"
#include "D2DRenderer.h"
#include "RigidBody2D.h"

const Bounds& BoxCollider2D::GetBox() const
{
	return m_box;
}

void BoxCollider2D::SetSize(const Vector2& size)
{
	m_box.extents = size;

	m_isColliderDirty = true;
}

void BoxCollider2D::Render()
{
	D2D1_COLOR_F color;
	if (m_isCollide)
	{
		color = D2D1::ColorF(D2D1::ColorF::Yellow);
	}
	else
	{
		color = D2D1::ColorF(D2D1::ColorF::Red);
	}


	Vector2 min = m_box.GetMin();
	Vector2 max = m_box.GetMax();

	D2D1_RECT_F rect{ min.x, min.y, max.x, max.y };

	D2DRenderer::Get()->DrawRect(rect, color);
}

void BoxCollider2D::UpdateCollider()
{
	if (m_rigidBody2d != nullptr)
	{
		m_box.center = m_rigidBody2d->GetPosition() + m_offset;
	}
	else
	{
		m_box.center = m_transform->GetWorldPosition() + m_offset;

		m_isColliderDirty = false;
	}
}

void BoxCollider2D::CalculateSpatialBounds()
{
	m_spatialBounds = m_box;
}

CollisionInfo BoxCollider2D::DetectCollision(const Collider* other) const
{
	return other->DetectCollision(this);
}

CollisionInfo BoxCollider2D::DetectCollision(const BoxCollider2D* other) const
{
	return Physics::DetectCollisionBoxBox(other, this);
}

CollisionInfo BoxCollider2D::DetectCollision(const CircleCollider* other) const
{
	return Physics::DetectCollisionCircleBox(other, this);
}

CollisionInfo BoxCollider2D::DetectCollision(const LineCollider* other) const
{
	return CollisionInfo(); // ¾È¾¸
}

CollisionInfo BoxCollider2D::DetectCollision(const ConeCollider2D* other) const
{
	return Physics::DetectCollisionConeBox(other, this);
}
