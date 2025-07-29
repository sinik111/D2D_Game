#include "pch.h"
#include "ConeCollider2D.h"

#include "Transform.h"

const Cone2D& ConeCollider2D::GetCone() const
{
    return m_cone;
}

void ConeCollider2D::SetCone(float radius, const Vector2& direction, float angle)
{
    m_cone.radius = radius;
    m_cone.direction = direction;
    m_cone.angle = angle;

    m_isColliderDirty = true;
}

void ConeCollider2D::UpdateCollider()
{
    m_cone.center = m_transform->GetWorldPosition() + m_offset;
}

void ConeCollider2D::CalculateSpatialBounds()
{
    m_spatialBounds = m_cone.GetBounds();
}

CollisionInfo ConeCollider2D::DetectCollision(const Collider* other) const
{
    return other->DetectCollision(this);
}

CollisionInfo ConeCollider2D::DetectCollision(const BoxCollider2D* other) const
{
    return Physics::DetectCollisionBoxCone(other, this);
}

CollisionInfo ConeCollider2D::DetectCollision(const CircleCollider* other) const
{
    return CollisionInfo(); // 필요없음
}

CollisionInfo ConeCollider2D::DetectCollision(const LineCollider* other) const
{
    return CollisionInfo(); // 필요없음
}

CollisionInfo ConeCollider2D::DetectCollision(const ConeCollider2D* other) const
{
    return Physics::DetectCollisionConeCone(other, this);
}
