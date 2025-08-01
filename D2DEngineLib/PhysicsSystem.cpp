#include "pch.h"
#include "PhysicsSystem.h"

#include "RigidBody2D.h"
#include "ContainerUtility.h"
#include "BoxCollider2D.h"
#include "D2DRenderer.h"
#include "Camera.h"

static const Bounds defaultBounds{ Vector2(0.0f, 0.0f), Vector2(400.0f, 300.0f) };
static const int defaultMaxDepth = 5;
static const int defaultmaxObjectsPerNode = 10;

PhysicsSystem::PhysicsSystem()
{
	m_quadtree = std::make_unique<Quadtree>(defaultBounds, defaultMaxDepth, defaultmaxObjectsPerNode);
}

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

void PhysicsSystem::RegisterCollider(Collider* collider)
{
	m_colliders.push_back(collider);

	if (collider->GetRigidBody2D() != nullptr)
	{
		m_rigidBodyColliders.push_back(collider);
	}

	m_quadtree->Insert(collider);
}

void PhysicsSystem::UnregisterCollider(Collider* collider)
{
	m_quadtree->Remove(collider);

	if (collider->GetRigidBody2D() != nullptr)
	{
		Util::OptimizedErase(m_rigidBodyColliders, collider);
	}

	Util::OptimizedErase(m_colliders, collider);
}

void PhysicsSystem::SetupQuadtree(const Bounds& worldBounds, int maxDepth, int maxObjectsPerNode)
{
	m_quadtree = std::make_unique<Quadtree>(worldBounds, maxDepth, maxObjectsPerNode);
}

void PhysicsSystem::SetD2DRenderer(D2DRenderer* d2dRenderer)
{
	m_d2dRenderer = d2dRenderer;
}

void PhysicsSystem::ProcessPhysics()
{
	ResetCollidersIsCollide();

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

	for (auto collider : m_rigidBodyColliders)
	{
		m_quadtree->Relocate(collider);
	}

	m_currentCollisions.clear();
	m_currentTriggers.clear();

	//for (auto collider : m_rigidBodyColliders) // no quadtree
	//{
	//	//auto candidates = m_quadtree->GetPotentialCollisions(collider);

	//	//for (auto candidate : candidates)
	//	for (auto candidate : m_colliders)
	//	{
	//		if (collider == candidate)
	//		{
	//			continue;
	//		}

	//		bool isInteractable = (Physics::GetCollisionMask(collider->GetLayer())
	//			& static_cast<unsigned int>(candidate->GetLayer())) != 0;

	//		if (isInteractable)
	//		{
	//			Collider* colliderA;
	//			Collider* colliderB;

	//			CollisionInfo info;

	//			if (collider < candidate)
	//			{
	//				colliderA = collider;
	//				colliderB = candidate;

	//				info = colliderB->DetectCollision(colliderA);
	//			}
	//			else
	//			{
	//				colliderA = candidate;
	//				colliderB = collider;

	//				info = colliderA->DetectCollision(colliderB);
	//			}

	//			if (info.isCollide)
	//			{
	//				colliderA->SetIsCollide(true);
	//				colliderB->SetIsCollide(true);

	//				CollisionPair pair{ colliderA, colliderB };

	//				if (colliderA->GetTrigger() || colliderB->GetTrigger())
	//				{
	//					m_currentTriggers.emplace(pair, info);
	//				}
	//				else
	//				{
	//					m_currentCollisions.emplace(pair, info);

	//					Physics::ResolveCollision(info);
	//				}
	//			}
	//		}
	//	}
	//}

	for (auto collider : m_rigidBodyColliders)
	{
		auto candidates = m_quadtree->GetPotentialCollisions(collider);

		for (auto candidate : candidates)
		{
			if (collider == candidate)
			{
				continue;
			}

			bool isInteractable = (Physics::GetCollisionMask(collider->GetLayer()) 
				& static_cast<unsigned int>(candidate->GetLayer())) != 0;

			if (isInteractable)
			{
				Collider* colliderA;
				Collider* colliderB;

				CollisionInfo info;

				if (collider < candidate)
				{
					colliderA = collider;
					colliderB = candidate;

					info = colliderB->DetectCollision(colliderA);
				}
				else
				{
					colliderA = candidate;
					colliderB = collider;

					info = colliderA->DetectCollision(colliderB);
				}

				if (info.isCollide)
				{
					colliderA->SetIsCollide(true);
					colliderB->SetIsCollide(true);

					CollisionPair pair{ colliderA, colliderB };

					if (colliderA->GetTrigger() || colliderB->GetTrigger())
					{
						m_currentTriggers.emplace(pair, info);
					}
					else
					{
						m_currentCollisions.emplace(pair, info);

						Physics::ResolveCollision(info);

						colliderA->UpdateCollider();
						colliderB->UpdateCollider();
					}
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

void PhysicsSystem::ResetCollidersIsCollide() // 디버그용
{
	for (const auto& collider : m_colliders)
	{
		collider->SetIsCollide(false);
	}
}

void PhysicsSystem::UpdateColliders()
{
	for (const auto& collider : m_colliders)
	{
		collider->Update();
	}
}

void PhysicsSystem::RenderColliders()
{
	m_d2dRenderer->GetDeviceContext()->SetTransform(Camera::s_mainCamera->GetViewMatrix() * m_d2dRenderer->GetUnityMatrix());

	auto allBounds = m_quadtree->GetAllNodeBounds();

	for (auto& bounds : allBounds)
	{
		Vector2 min = bounds.GetMin();
		Vector2 max = bounds.GetMax();

		D2D1_RECT_F rect{ min.x, min.y, max.x, max.y };

		D2DRenderer::Get()->DrawRect(rect);
	}
	
	for (const auto& collider : m_colliders)
	{
		collider->Render();
	}
}

void PhysicsSystem::ClearCollisionPairs()
{
	m_quadtree->Clear();

	m_currentCollisions.clear();
	m_previousCollisions.clear();

	m_currentTriggers.clear();
	m_previousCollisions.clear();
}

void PhysicsSystem::CallCollisionEvent()
{
	for (const auto& pair : m_currentCollisions)
	{
		Collision collisionA{
			pair.second.colliderB->GetGameObject(),
			pair.second.colliderB,
			-pair.second.normal,
			pair.second.penetrationDepth,
			pair.second.contactPoint
		};

		Collision collisionB{
			pair.second.colliderA->GetGameObject(),
			pair.second.colliderA,
			pair.second.normal,
			pair.second.penetrationDepth,
			pair.second.contactPoint
		};

		if (m_previousCollisions.find(pair.first) == m_previousCollisions.end())
		{
			pair.first.first->CallOnCollisionEnter(collisionB);
			pair.first.second->CallOnCollisionEnter(collisionA);
		}
		else
		{
			pair.first.first->CallOnCollisionStay(collisionB);
			pair.first.second->CallOnCollisionStay(collisionA);
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
					-pair.second.normal,
					pair.second.penetrationDepth,
					pair.second.contactPoint
				};

				Collision collisionB{
					pair.second.colliderA->GetGameObject(),
					pair.second.colliderA,
					pair.second.normal,
					pair.second.penetrationDepth,
					pair.second.contactPoint
				};

				pair.first.second->CallOnCollisionExit(collisionA);
				pair.first.first->CallOnCollisionExit(collisionB);
			}
			else if (validA)
			{
				Collision collisionA{
					nullptr,
					nullptr,
					-pair.second.normal,
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
					pair.second.normal,
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
			-pair.second.normal,
			pair.second.penetrationDepth,
			pair.second.contactPoint
		};

		Collision collisionB{
			pair.second.colliderA->GetGameObject(),
			pair.second.colliderA,
			pair.second.normal,
			pair.second.penetrationDepth,
			pair.second.contactPoint
		};

		if (m_previousTriggers.find(pair.first) == m_previousTriggers.end())
		{
			pair.first.first->CallOnTriggerEnter(collisionB);
			pair.first.second->CallOnTriggerEnter(collisionA);
		}
		else
		{
			pair.first.first->CallOnTriggerStay(collisionB);
			pair.first.second->CallOnTriggerStay(collisionA);
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
					-pair.second.normal,
					pair.second.penetrationDepth,
					pair.second.contactPoint
				};

				Collision collisionB{
					pair.second.colliderA->GetGameObject(),
					pair.second.colliderA,
					pair.second.normal,
					pair.second.penetrationDepth,
					pair.second.contactPoint
				};

				pair.first.second->CallOnTriggerExit(collisionA);
				pair.first.first->CallOnTriggerExit(collisionB);
			}
			else if (validA)
			{
				Collision collisionA{
					nullptr,
					nullptr,
					-pair.second.normal,
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
					pair.second.normal,
					pair.second.penetrationDepth,
					pair.second.contactPoint
				};

				pair.first.second->CallOnTriggerExit(collisionB);
			}
		}
	}

	m_previousTriggers = m_currentTriggers;
}
