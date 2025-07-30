#include "pch.h"
#include "SectorCollider.h"

#include "Transform.h"

const Sector& SectorCollider::GetSector() const
{
    return m_sector;
}

void SectorCollider::SetSector(float radius, const Vector2& direction, float angle)
{
    m_sector.radius = radius;
    m_sector.direction = direction;
    m_sector.angle = angle;

    m_isColliderDirty = true;
}

void SectorCollider::UpdateCollider()
{
    m_sector.center = m_transform->GetWorldPosition() + m_offset;
}

void SectorCollider::CalculateSpatialBounds()
{
    m_spatialBounds = m_sector.GetBounds();
}

CollisionInfo SectorCollider::DetectCollision(const Collider* other)
{
    return CollisionInfo();
}

CollisionInfo SectorCollider::DetectCollision(const BoxCollider2D* other)
{
    return CollisionInfo();
}

CollisionInfo SectorCollider::DetectCollision(const CircleCollider* other)
{
    return CollisionInfo();
}

CollisionInfo SectorCollider::DetectCollision(const LineCollider* other)
{
    return CollisionInfo();
}

CollisionInfo SectorCollider::DetectCollision(const SectorCollider* other)
{
    return CollisionInfo();
}
