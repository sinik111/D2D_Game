#include "pch.h"
#include "CircleCollider.h"

#include "Transform.h"

const Circle& CircleCollider::GetCircle() const
{
    return m_circle;
}

void CircleCollider::SetRadius(float radius)
{
    m_circle.radius = radius;

    m_isColliderDirty = true;
}

void CircleCollider::UpdateCollider()
{
    m_circle.center = m_transform->GetWorldPosition() + m_offset;
}

void CircleCollider::CalculateSpatialBounds()
{
    m_spatialBounds = m_circle.GetBounds();
}

CollisionInfo CircleCollider::DetectCollision(const Collider* other) const
{
    return other->DetectCollision(this);
}

CollisionInfo CircleCollider::DetectCollision(const BoxCollider2D* other) const
{
    return Physics::DetectCollisionBoxCircle(other, this);
}

CollisionInfo CircleCollider::DetectCollision(const CircleCollider* other) const
{
    return Physics::DetectCollisionCircleCircle(other, this);
}

CollisionInfo CircleCollider::DetectCollision(const LineCollider* other) const
{
    return Physics::DetectCollisionLineCircle(other, this);
}

CollisionInfo CircleCollider::DetectCollision(const ConeCollider2D* other) const
{
    return CollisionInfo(); // ¾È¾¸
}
