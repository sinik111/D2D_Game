#include "pch.h"
#include "LineCollider.h"

#include "Transform.h"
#include "D2DRenderer.h"

const LineSegment& LineCollider::GetLineSegment() const
{
    return m_lineSegment;
}

void LineCollider::SetLine(const Vector2& startPoint, const Vector2& endPoint)
{
    m_localLineSegment = LineSegment(startPoint, endPoint);

    m_isColliderDirty = true;
}

void LineCollider::Render()
{
    Vector2 position = m_transform->GetWorldPosition();

    D2D1_COLOR_F color;
    if (m_isCollide)
    {
        color = D2D1::ColorF(D2D1::ColorF::Yellow);
    }
    else
    {
        color = D2D1::ColorF(D2D1::ColorF::Red);
    }

    D2DRenderer::Get()->DrawLine(
        { m_lineSegment.startPoint.x, m_lineSegment.startPoint.y },
        { m_lineSegment.endPoint.x, m_lineSegment.endPoint.y },
        color
    );

    D2DRenderer::Get()->DrawLine(
        { position.x, position.y },
        { position.x + m_lineSegment.normal.x * 20.0f, position.y + m_lineSegment.normal.y * 20.0f },
        color
    );
}

void LineCollider::UpdateCollider()
{
    Vector2 position = m_transform->GetWorldPosition();

    m_lineSegment = m_localLineSegment;
    m_lineSegment.startPoint += position;
    m_lineSegment.endPoint += position;
}

void LineCollider::CalculateSpatialBounds()
{
    m_spatialBounds = m_lineSegment.GetBounds();
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
