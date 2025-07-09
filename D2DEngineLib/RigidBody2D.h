#pragma once

#include "Component.h"

class Transform;

enum class BodyType
{
	Dynamic,
	Kinematic,
	Static
};

class RigidBody2D :
	public Component
{
private:
	Transform* m_transform;

	BodyType m_bodyType;
	float m_mass;
	
};