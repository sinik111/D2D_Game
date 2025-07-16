#pragma once

class BoxCollider2D;
class RigidBody2D;
class GameObject;

struct Collision
{
	const GameObject* otherGameObject;
	const BoxCollider2D* other;
	const Vector2 normal;
	const float penetrationDepth;
	const Vector2 contactPoint;
};

struct CollisionInfo
{
	bool isCollide = false;
	const BoxCollider2D* colliderA = nullptr;
	const BoxCollider2D* colliderB = nullptr;

	Vector2 normal;
	float penetrationDepth;
	Vector2 contactPoint;
	Vector2 relativeVelocity;

	RigidBody2D* rigidBodyA = nullptr;
	RigidBody2D* rigidBodyB = nullptr;
};

class Physics
{
public:
	static const Vector2 gravity;

	static bool DetectCollision(const BoxCollider2D* a, const BoxCollider2D* b, CollisionInfo& info);

	static void ResolveCollision(const CollisionInfo& info);
};