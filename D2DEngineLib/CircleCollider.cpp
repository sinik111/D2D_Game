#include "pch.h"
#include "CircleCollider.h"

#include "Transform.h"
#include "D2DRenderer.h"
#include "RigidBody2D.h"

const Circle& CircleCollider::GetCircle() const
{
    return m_circle;
}

void CircleCollider::SetRadius(float radius)
{
    m_circle.radius = radius;

    m_isColliderDirty = true;
}

void CircleCollider::Render()
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


    D2D1_ELLIPSE ellipse{ { m_circle.center.x, m_circle.center.y }, m_circle.radius, m_circle.radius };

    D2DRenderer::Get()->DrawEllipse(ellipse, color);
}

void CircleCollider::UpdateCollider()
{
    if (m_rigidBody2d != nullptr)
    {
        m_circle.center = m_rigidBody2d->GetPosition() + m_offset;
    }
    else
    {
        m_circle.center = m_transform->GetWorldPosition() + m_offset;
    }
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
