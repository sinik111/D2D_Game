#pragma once

#include "Physics.h"
#include "Quadtree.h"

class RigidBody2D;
class Collider;
class D2DRenderer;

class PhysicsSystem
{
private:
	std::vector<RigidBody2D*> m_dynamicRigidBodies;
	std::vector<RigidBody2D*> m_kinematicRigidBodies;
	std::vector<RigidBody2D*> m_staticRigidBodies;
	std::vector<Collider*> m_colliders;
	std::vector<Collider*> m_rigidBodyColliders;

	using CollisionPair = std::pair<Collider*, Collider*>;

	std::unordered_map<CollisionPair, CollisionInfo, Util::PairHash> m_previousCollisions;
	std::unordered_map<CollisionPair, CollisionInfo, Util::PairHash> m_currentCollisions;

	std::unordered_map<CollisionPair, CollisionInfo, Util::PairHash> m_previousTriggers;
	std::unordered_map<CollisionPair, CollisionInfo, Util::PairHash> m_currentTriggers;

	D2DRenderer* m_d2dRenderer = nullptr;

	std::unique_ptr<Quadtree> m_quadtree;

public:
	PhysicsSystem();

	void RegisterRigidBody2D(RigidBody2D* rigidBody2d);
	void UnregisterRigidBody2D(RigidBody2D* rigidBody2d);
	void RegisterCollider(Collider* collider);
	void UnregisterCollider(Collider* collider);

public:
	void SetupQuadtree(const Bounds& worldBounds, int maxDepth, int maxObjectsPerNode);
	void SetD2DRenderer(D2DRenderer* d2dRenderer);
	void ProcessPhysics();

	void Interpolate();
	void ResetCollidersIsCollide();
	void UpdateColliders();

	void RenderColliders();

	void ClearCollisionPairs();

private:
	void CallCollisionEvent();
	void CallTriggerEvent();
};