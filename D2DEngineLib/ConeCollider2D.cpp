#include "pch.h"
#include "ConeCollider2D.h"

#include "Transform.h"
#include "D2DRenderer.h"
#include "RigidBody2D.h"

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

void ConeCollider2D::Render()
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


    Vector2 vertices[3];

    m_cone.GetVertices(vertices);

    D2DRenderer::Get()->DrawTriangle(
        { vertices[0].x, vertices[0].y },
        { vertices[1].x, vertices[1].y },
        { vertices[2].x, vertices[2].y },
        color
    );
}

void ConeCollider2D::UpdateCollider()
{
    if (m_rigidBody2d != nullptr)
    {
        m_cone.center = m_rigidBody2d->GetPosition() + m_offset;
    }
    else
    {
        m_cone.center = m_transform->GetWorldPosition() + m_offset;

        m_isColliderDirty = false;
    }
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
