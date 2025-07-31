#pragma once

#include "CollisionLayer.h"

class Collider;
class BoxCollider2D;
class CircleCollider;
class LineCollider;
class ConeCollider2D;

class RigidBody2D;
class GameObject;

struct Circle;
struct LineSegment;

struct Collision
{
	const GameObject* otherGameObject;
	const Collider* other;
	const Vector2 normal;
	const float penetrationDepth;
	const Vector2 contactPoint;
};

struct CollisionInfo
{
	const Collider* colliderA = nullptr;
	const Collider* colliderB = nullptr;

	Vector2 normal;
	float penetrationDepth;
	Vector2 contactPoint;

	RigidBody2D* rigidBodyA = nullptr;
	RigidBody2D* rigidBodyB = nullptr;

	bool isCollide = false;
};

struct Projection
{
	float min;
	float max;

	bool Overlaps(const Projection& other) const
	{
		return !(min > other.max || other.min > max);
		//return max > other.min && other.max > min;
	}

	float GetOverlap(const Projection& other) const
	{
		if (!Overlaps(other))
		{
			return 0.0f;
		}

		return std::min<float>(max, other.max) - std::max<float>(min, other.min);
	}
};

class Physics
{
private:
	static std::unordered_map<CollisionLayer, unsigned int> s_collisionMasks;

public:
	static const Vector2 gravity;

	static void SetupCollisionMatrix();
	static unsigned int GetCollisionMask(CollisionLayer layer);

	static bool DetectCollision(const BoxCollider2D* a, const BoxCollider2D* b, CollisionInfo& info);

	static CollisionInfo DetectCollisionBoxBox(const BoxCollider2D* a, const BoxCollider2D* b);
	static CollisionInfo DetectCollisionCircleCircle(const CircleCollider* a, const CircleCollider* b);
	static CollisionInfo DetectCollisionConeCone(const ConeCollider2D* a, const ConeCollider2D* b);

	static CollisionInfo DetectCollisionBoxCircle(const BoxCollider2D* a, const CircleCollider* b);
	static CollisionInfo DetectCollisionCircleBox(const CircleCollider* a, const BoxCollider2D* b);

	static CollisionInfo DetectCollisionLineCircle(const LineCollider* a, const CircleCollider* b);
	static CollisionInfo DetectCollisionCircleLine(const CircleCollider* a, const LineCollider* b);

	static CollisionInfo DetectCollisionBoxCone(const BoxCollider2D* a, const ConeCollider2D* b);
	static CollisionInfo DetectCollisionConeBox(const ConeCollider2D* a, const BoxCollider2D* b);

	static Projection ProjectPolygon(const Vector2* vertices, int numVertices, const Vector2& axis);
	static Vector2 GetClosestPointOnLineSegment(const Vector2& point, const LineSegment& segment, float& t_out);

	static void ResolveCollision(const CollisionInfo& info);
};