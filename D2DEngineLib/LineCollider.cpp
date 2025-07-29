#include "pch.h"
#include "LineCollider.h"

#include "Transform.h"

const LineSegment& LineCollider::GetLineSegment() const
{
    return m_lineSegment;
}

void LineCollider::SetLine(const Vector2& startPoint, const Vector2& endPoint)
{
    m_lineSegment = LineSegment(startPoint, endPoint);

    m_isColliderDirty = true;
}

void LineCollider::UpdateCollider()
{

}

void LineCollider::CalculateSpatialBounds()
{
    m_spatialBounds = m_lineSegment.GetBounds(m_transform->GetWorldPosition());
}

CollisionInfo LineCollider::DetectCollision(const Collider* other) const
{
    return other->DetectCollision(this);
}

CollisionInfo LineCollider::DetectCollision(const BoxCollider2D* other) const
{
    return CollisionInfo(); // ¾È¾¸
}

CollisionInfo LineCollider::DetectCollision(const CircleCollider* other) const
{
    return Physics::DetectCollisionCircleLine(other, this);
}

CollisionInfo LineCollider::DetectCollision(const LineCollider* other) const
{
    return CollisionInfo(); // ¾È¾¸
}

CollisionInfo LineCollider::DetectCollision(const ConeCollider2D* other) const
{
    return CollisionInfo(); // ¾È¾¸
}
