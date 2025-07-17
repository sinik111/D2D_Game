#include "pch.h"
#include "PhysicsSystem.h"

#include "RigidBody2D.h"
#include "ContainerUtility.h"
#include "BoxCollider2D.h"
#include "D2DRenderer.h"

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

void PhysicsSystem::SetD2DRenderer(D2DRenderer* d2dRenderer)
{
	m_d2dRenderer = d2dRenderer;
}

void PhysicsSystem::ProcessPhysics()
{
	for (const auto& rigidBody2d : m_dynamicRigidBodies)
	{
		rigidBody2d->ApplyGraviy(Physics::gravity);

		rigidBody2d->CalculatePosition();
	}

	for (const auto& rigidBody2d : m_kinematicRigidBodies)
	{
		rigidBody2d->CalculatePosition();
	}

	UpdateColliders();
	
	std::sort(
		m_boxColliders.begin(),
		m_boxColliders.end(),
		[](BoxCollider2D* a, BoxCollider2D* b) {
			if (a->GetRigidBody2D() == nullptr || b->GetRigidBody2D() == nullptr)
			{
				return false;
			}

			return a->GetRigidBody2D()->GetPosition().y > b->GetRigidBody2D()->GetPosition().y;
		}
	);

	m_currentCollisions.clear();
	m_currentTriggers.clear();

	for (size_t i = 0; i < m_boxColliders.size(); ++i)
	{
		for (size_t j = i + 1; j < m_boxColliders.size(); ++j)
		{
			if (m_boxColliders[i]->GetRigidBody2D() == nullptr &&
				m_boxColliders[j]->GetRigidBody2D() == nullptr)
			{
				continue;
			}

			BoxCollider2D* colliderA;
			BoxCollider2D* colliderB;

			if (m_boxColliders[i] < m_boxColliders[j])
			{
				colliderA = m_boxColliders[i];
				colliderB = m_boxColliders[j];
			}
			else
			{
				colliderA = m_boxColliders[j];
				colliderB = m_boxColliders[i];
			}

			CollisionInfo info{};

			if (Physics::DetectCollision(colliderA, colliderB, info))
			{
				CollisionPair pair{ colliderA, colliderB };

				if (colliderA->GetTrigger() || colliderB->GetTrigger())
				{
					m_currentTriggers.emplace(pair, info);
				}
				else
				{
					m_currentCollisions.emplace(pair, info);

					Physics::ResolveCollision(info);
				}
			}
		}
	}

	// Enter, Stay, Exit 상태 처리
	CallCollisionEvent();
	CallTriggerEvent();
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

void PhysicsSystem::RenderColliders()
{
	for (const auto& collider : m_boxColliders)
	{
		const Bounds& bounds = collider->GetBounds();

		Vector2 min = bounds.GetMin();
		Vector2 max = bounds.GetMax();

		D2D1_RECT_F rect{ min.x, min.y, max.x, max.y };

		m_d2dRenderer->DrawRect(rect);
	}
}

void PhysicsSystem::CallCollisionEvent()
{
	for (const auto& pair : m_currentCollisions)
	{
		Collision collisionA{
			pair.second.colliderB->GetGameObject(),
			pair.second.colliderB,
			pair.second.normal,
			pair.second.penetrationDepth,
			pair.second.contactPoint
		};

		Collision collisionB{
			pair.second.colliderA->GetGameObject(),
			pair.second.colliderA,
			-pair.second.normal,
			pair.second.penetrationDepth,
			pair.second.contactPoint
		};

		if (m_previousCollisions.find(pair.first) == m_previousCollisions.end())
		{
			pair.first.first->CallOnCollisionEnter(collisionA);
			pair.first.second->CallOnCollisionEnter(collisionB);
		}
		else
		{
			pair.first.first->CallOnCollisionStay(collisionA);
			pair.first.second->CallOnCollisionStay(collisionB);
		}
	}

	for (const auto& pair : m_previousCollisions)
	{
		if (m_currentCollisions.find(pair.first) == m_currentCollisions.end())
		{
			bool validA = Object::IsValid(pair.first.first);
			bool validB = Object::IsValid(pair.first.second);

			if (validA && validB)
			{
				Collision collisionA{
					pair.second.colliderB->GetGameObject(),
					pair.second.colliderB,
					pair.second.normal,
					pair.second.penetrationDepth,
					pair.second.contactPoint
				};

				pair.first.first->CallOnCollisionExit(collisionA);

				Collision collisionB{
					pair.second.colliderA->GetGameObject(),
					pair.second.colliderA,
					-pair.second.normal,
					pair.second.penetrationDepth,
					pair.second.contactPoint
				};

				pair.first.second->CallOnCollisionExit(collisionB);
			}
			else if (validA)
			{
				Collision collisionA{
					nullptr,
					nullptr,
					pair.second.normal,
					pair.second.penetrationDepth,
					pair.second.contactPoint
				};

				pair.first.first->CallOnCollisionExit(collisionA);
			}
			else if (validB)
			{
				Collision collisionB{
					nullptr,
					nullptr,
					-pair.second.normal,
					pair.second.penetrationDepth,
					pair.second.contactPoint
				};

				pair.first.second->CallOnCollisionExit(collisionB);
			}
		}
	}

	m_previousCollisions = m_currentCollisions;
}

void PhysicsSystem::CallTriggerEvent()
{
	for (const auto& pair : m_currentTriggers)
	{
		Collision collisionA{
			pair.second.colliderB->GetGameObject(),
			pair.second.colliderB,
			pair.second.normal,
			pair.second.penetrationDepth,
			pair.second.contactPoint
		};

		Collision collisionB{
			pair.second.colliderA->GetGameObject(),
			pair.second.colliderA,
			-pair.second.normal,
			pair.second.penetrationDepth,
			pair.second.contactPoint
		};

		if (m_previousTriggers.find(pair.first) == m_previousTriggers.end())
		{
			pair.first.first->CallOnTriggerEnter(collisionA);
			pair.first.second->CallOnTriggerEnter(collisionB);
		}
		else
		{
			pair.first.first->CallOnTriggerStay(collisionA);
			pair.first.second->CallOnTriggerStay(collisionB);
		}
	}

	for (const auto& pair : m_previousTriggers)
	{
		if (m_currentTriggers.find(pair.first) == m_currentTriggers.end())
		{
			bool validA = Object::IsValid(pair.first.first);
			bool validB = Object::IsValid(pair.first.second);

			if (validA && validB)
			{
				Collision collisionA{
					pair.second.colliderB->GetGameObject(),
					pair.second.colliderB,
					pair.second.normal,
					pair.second.penetrationDepth,
					pair.second.contactPoint
				};

				pair.first.first->CallOnTriggerExit(collisionA);

				Collision collisionB{
					pair.second.colliderA->GetGameObject(),
					pair.second.colliderA,
					-pair.second.normal,
					pair.second.penetrationDepth,
					pair.second.contactPoint
				};

				pair.first.second->CallOnTriggerExit(collisionB);
			}
			else if (validA)
			{
				Collision collisionA{
					nullptr,
					nullptr,
					pair.second.normal,
					pair.second.penetrationDepth,
					pair.second.contactPoint
				};

				pair.first.first->CallOnTriggerExit(collisionA);
			}
			else if (validB)
			{
				Collision collisionB{
					nullptr,
					nullptr,
					-pair.second.normal,
					pair.second.penetrationDepth,
					pair.second.contactPoint
				};

				pair.first.second->CallOnTriggerExit(collisionB);
			}
		}
	}

	m_previousTriggers = m_currentTriggers;
}
