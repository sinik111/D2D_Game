#pragma once

#include "Component.h"

class Transform;

class RigidBody2D :
	public Component
{
public:
	enum class BodyType
	{
		Dynamic,
		Kinematic,
		Static
	};

private:
	Transform* m_transform = nullptr;

	BodyType m_bodyType = BodyType::Dynamic;
	float m_mass = 1.0f;
	float m_gravityScale = 1.0f;
	float m_accumulatedTime = 0.0f;
	
	Vector2 m_accumulatedForce;
	Vector2 m_velocity;

	Vector2 m_lastFramePosition;
	Vector2 m_position;

	bool m_isMultipleCalculation = false;

public:
	void Initialize() override;
	void RegisterToSystem() override;
	void UnregisterFromSystem() override;

public:
	void SetBodyType(BodyType bodyType);
	void SetMass(float mass);
	void SetGravityScale(float gravityScale);
	void SetVelocity(const Vector2& velocity);
	void SetPosition(const Vector2& position);
	void AddForce(const Vector2& force);


public:
	BodyType GetBodyType() const;
	Vector2 GetVelocity() const;
	Vector2 GetPosition() const;
	float GetGravityScale() const;
	float GetMass() const;

public:
	void CalculatePosition();
	void ApplyGraviy(const Vector2& gravity);
	void Interpolate();
};