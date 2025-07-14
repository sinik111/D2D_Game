#pragma once

#include "Component.h"

class RigidBody2D;

class BoxCollider2D :
	public Component
{
private:
	Transform* m_transform = nullptr;
	RigidBody2D* m_rigidBody2d = nullptr;

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
};