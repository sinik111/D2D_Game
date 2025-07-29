#include "pch.h"
#include "BoxCollider2D.h"

#include "Transform.h"

const Bounds& BoxCollider2D::GetBox() const
{
	return m_box;
}

void BoxCollider2D::SetSize(const Vector2& size)
{
	m_box.extents = size;

	m_isColliderDirty = true;
}

void BoxCollider2D::UpdateCollider()
{
	m_box.center = m_transform->GetWorldPosition() + m_offset;
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
