#include "pch.h"
#include "RigidBody2D.h"

#include "ComponentSystem.h"
#include "Transform.h"

void RigidBody2D::Initialize()
{
    m_transform = GetTransform();

    m_lastFramePosition = m_position = m_transform->GetLocalPosition();
}

void RigidBody2D::RegisterToSystem()
{
    ComponentSystem::Get().Physics().RegisterRigidBody2D(this);
}

void RigidBody2D::UnregisterFromSystem()
{
    ComponentSystem::Get().Physics().UnregisterRigidBody2D(this);
}

void RigidBody2D::SetBodyType(BodyType bodyType)
{
    m_bodyType = bodyType;
}

void RigidBody2D::SetMass(float mass)
{
    m_mass = mass;
}

void RigidBody2D::SetGravityScale(float gravityScale)
{
    m_gravityScale = gravityScale;
}

void RigidBody2D::SetVelocity(const Vector2& velocity)
{
    m_velocity = velocity;
}

void RigidBody2D::SetPosition(const Vector2& position)
{
    m_lastFramePosition = m_position = position;
}

void RigidBody2D::AddForce(const Vector2& force)
{
    m_accumulatedForce += force;
}

void RigidBody2D::ApplyGraviy(const Vector2& gravity)
{
    m_accumulatedForce += gravity * m_gravityScale * m_mass;
}

void RigidBody2D::Interpolate()
{
    m_accumulatedTime += MyTime::DeltaTime();

    Vector2 interpolatedPosition = Vector2::Lerp(
        m_lastFramePosition,
        m_position,
        m_accumulatedTime / MyTime::FixedDeltaTime()
    );

    m_transform->SetLocalPosition(interpolatedPosition);
}

RigidBody2D::BodyType RigidBody2D::GetBodyType() const
{
    return m_bodyType;
}

Vector2 RigidBody2D::GetVelocity() const
{
    return m_velocity;
}

Vector2 RigidBody2D::GetPosition() const
{
    return m_position;
}

float RigidBody2D::GetGravityScale() const
{
    return m_gravityScale;
}

float RigidBody2D::GetMass() const
{
    return m_mass;
}

void RigidBody2D::CalculatePosition()
{
    m_velocity += m_accumulatedForce / m_mass * MyTime::FixedDeltaTime();

    m_lastFramePosition = m_position;

    m_position += m_velocity * MyTime::FixedDeltaTime();

    m_accumulatedForce = Vector2::Zero;

    m_accumulatedTime = 0.0f;
}