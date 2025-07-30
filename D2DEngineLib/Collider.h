#pragma once

#include "Component.h"
#include "Bounds.h"
#include "Physics.h"
#include "Delegate.h"
#include "CollisionLayer.h"

class BoxCollider2D;
class CircleCollider;
class LineCollider;
class ConeCollider2D;

class Transform;
class RigidBody2D;
class Script;

class Collider :
	public Component
{
protected:
	Transform* m_transform = nullptr;
	RigidBody2D* m_rigidBody2d = nullptr;

	std::vector<Script*> m_scriptsForCallBack;

	Bounds m_spatialBounds; // 쿼드트리 공간 분할용 AABB

	Vector2 m_offset;

	CollisionLayer m_layer;

	bool m_isTrigger = false;
	bool m_isColliderDirty = true;
	bool m_isCollide = false;

public:
	void Initialize() override;
	void RegisterToSystem() override;
	void UnregisterFromSystem() override;

	void RegisterScript(Script* script);
	void UnregisterScript(Script* script);

	RigidBody2D* GetRigidBody2D() const;
	virtual void CalculateSpatialBounds() = 0;
	void SetTrigger(bool trigger);
	void SetOffset(const Vector2& offset);
	void SetLayer(CollisionLayer layer);
	void SetIsCollide(bool isCollide);

	bool GetTrigger() const;
	Bounds GetSpatialBounds() const;
	CollisionLayer GetLayer() const;

	void Update();
	virtual void UpdateCollider() = 0;
	virtual void Render();

public:
	virtual CollisionInfo DetectCollision(const Collider* other) const = 0;
	virtual CollisionInfo DetectCollision(const BoxCollider2D* other) const = 0;
	virtual CollisionInfo DetectCollision(const CircleCollider* other) const = 0;
	virtual CollisionInfo DetectCollision(const LineCollider* other) const = 0;
	virtual CollisionInfo DetectCollision(const ConeCollider2D* other) const = 0;

	virtual void CallOnCollisionEnter(const Collision& collision);
	virtual void CallOnCollisionStay(const Collision& collision);
	virtual void CallOnCollisionExit(const Collision& collision);
	virtual void CallOnTriggerEnter(const Collision& collision);
	virtual void CallOnTriggerStay(const Collision& collision);
	virtual void CallOnTriggerExit(const Collision& collision);
};