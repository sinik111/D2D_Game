#pragma once

#include "Component.h"
#include "Delegate.h"
#include "Physics.h"

class RigidBody2D;
class Script;

class BoxCollider2D :
	public Component
{
private:
	Transform* m_transform = nullptr;
	RigidBody2D* m_rigidBody2d = nullptr;

	std::vector<Script*> m_scriptsForCallBack;

	Bounds m_bounds;
	Vector2 m_offset;
	Vector2 m_size;
	bool m_isBoundsDirty = true;
	bool m_isTrigger = false;

public:
	BoxCollider2D() = default;
	~BoxCollider2D() override = default;

public:
	void Initialize() override;
	void RegisterToSystem() override;
	void UnregisterFromSystem() override;

	void RegisterScript(Script* script);
	void UnregisterScript(Script* script);

public:
	RigidBody2D* GetRigidBody2D() const;

public:
	bool GetTrigger() const;
	const Bounds& GetBounds() const;

public:
	void SetOffset(const Vector2& offset);
	void SetSize(const Vector2& size);
	void SetTrigger(bool trigger);

public:
	void Update();

	void CallOnCollisionEnter(const CollisionInfo& collisionInfo);
	void CallOnCollisionStay(const CollisionInfo& collisionInfo);
	void CallOnCollisionExit(const CollisionInfo& collisionInfo);
	void CallOnTriggerEnter(const CollisionInfo& collisionInfo);
	void CallOnTriggerStay(const CollisionInfo& collisionInfo);
	void CallOnTriggerExit(const CollisionInfo& collisionInfo);
};