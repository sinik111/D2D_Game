#include "pch.h"
#include "PhysicsSystem.h"

#include "RigidBody2D.h"
#include "ContainerUtility.h"
#include "Physics.h"
#include "BoxCollider2D.h"

void PhysicsSystem::RegisterRigidBody2D(RigidBody2D* rigidBody2d)
{
	switch (rigidBody2d->GetBodyType())
	{
	case RigidBody2D::BodyType::Dynamic:
		m_dynamicRigidBodies.push_back(rigidBody2d);
		break;

	case RigidBody2D::BodyType::Kinematic:
		m_kinematicRigidBodies.push_back(rigidBody2d);
		break;

	case RigidBody2D::BodyType::Static:
		m_staticRigidBodies.push_back(rigidBody2d);
		break;
	}
}

void PhysicsSystem::UnregisterRigidBody2D(RigidBody2D* rigidBody2d)
{
	switch (rigidBody2d->GetBodyType())
	{
	case RigidBody2D::BodyType::Dynamic:
		Util::OptimizedErase(m_dynamicRigidBodies, rigidBody2d);
		break;

	case RigidBody2D::BodyType::Kinematic:
		Util::OptimizedErase(m_kinematicRigidBodies, rigidBody2d);
		break;

	case RigidBody2D::BodyType::Static:
		Util::OptimizedErase(m_staticRigidBodies, rigidBody2d);
		break;
	}
}

void PhysicsSystem::RegisterBoxCollider2D(BoxCollider2D* boxCollider2d)
{
	m_boxColliders.push_back(boxCollider2d);
}

void PhysicsSystem::UnregisterBoxCollider2D(BoxCollider2D* boxCollider2d)
{
	Util::OptimizedErase(m_boxColliders, boxCollider2d);
}

void PhysicsSystem::ProcessPhysics()
{
	m_accumulatedDeltaTime += MyTime::DeltaTime();

	const float fixedDeltaTime = MyTime::FixedDeltaTime();

	while (m_accumulatedDeltaTime >= fixedDeltaTime)
	{
		m_accumulatedDeltaTime -= fixedDeltaTime;

		for (const auto& rigidBody2d : m_dynamicRigidBodies)
		{
			rigidBody2d->ApplyOverriden();
		}

		for (const auto& rigidBody2d : m_kinematicRigidBodies)
		{
			rigidBody2d->ApplyOverriden();
		}

		for (const auto& rigidBody2d : m_staticRigidBodies)
		{
			rigidBody2d->ApplyOverriden();
		}

		for (const auto& rigidBody2d : m_dynamicRigidBodies)
		{
			rigidBody2d->ApplyGraviy(Physics::gravity);

			rigidBody2d->CalculatePosition();
		}

		for (size_t i = 0; i < m_boxColliders.size(); ++i)
		{
			for (size_t j = i + 1; j < m_boxColliders.size(); ++j)
			{
				CollisionInfo info{};

				if (Physics::DetectCollision(m_boxColliders[i], m_boxColliders[j], info))
				{
					Physics::ResolveCollision(info);
				}
			}
		}
	}
}

void PhysicsSystem::Interpolate()
{
	for (const auto& rigidBody2d : m_dynamicRigidBodies)
	{
		rigidBody2d->Interpolate();
	}

	for (const auto& rigidBody2d : m_kinematicRigidBodies)
	{
		rigidBody2d->Interpolate();
	}

	for (const auto& rigidBody2d : m_staticRigidBodies)
	{
		rigidBody2d->Interpolate();
	}
}

void PhysicsSystem::UpdateColliders()
{
	for (const auto& collider : m_boxColliders)
	{
		collider->Update();
	}
}
