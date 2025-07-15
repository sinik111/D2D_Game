#include "pch.h"
#include "BoxCollider2D.h"

#include "GameObject.h"
#include "RigidBody2D.h"
#include "ComponentSystem.h"
#include "ContainerUtility.h"

void BoxCollider2D::Initialize()
{
	m_transform = GetTransform();
	m_rigidBody2d = GetGameObject()->GetComponent<RigidBody2D>();
}

void BoxCollider2D::RegisterToSystem()
{
	ComponentSystem::Get().Physics().RegisterBoxCollider2D(this);
}

void BoxCollider2D::UnregisterFromSystem()
{
	ComponentSystem::Get().Physics().UnregisterBoxCollider2D(this);
}

void BoxCollider2D::RegisterScript(Script* script)
{
	m_scriptsForCallBack.push_back(script);
}

void BoxCollider2D::UnregisterScript(Script* script)
{
	Util::OptimizedErase(m_scriptsForCallBack, script);
}

RigidBody2D* BoxCollider2D::GetRigidBody2D() const
{
	return m_rigidBody2d;
}

bool BoxCollider2D::GetTrigger() const
{
	return m_isTrigger;
}

const Bounds& BoxCollider2D::GetBounds() const
{
	return m_bounds;
}

void BoxCollider2D::SetOffset(const Vector2& offset)
{
	m_offset = offset;

	m_isBoundsDirty = true;
}

void BoxCollider2D::SetSize(const Vector2& size)
{
	m_size = size;

	m_isBoundsDirty = true;
}

void BoxCollider2D::SetTrigger(bool trigger)
{
	m_isTrigger = trigger;
}

void BoxCollider2D::Update()
{
	if (m_isBoundsDirty || m_transform->GetIsDirtyThisFrame())
	{
		// 현재는 AABB만 가능하도록 회전, 스케일은 제외함
		// 스케일은 회전에서 분리해야하기 때문에 그냥 뺌.

		if (m_rigidBody2d != nullptr)
		{
			m_bounds = Bounds(
				m_rigidBody2d->GetPosition() + m_offset,
				m_size);
		}
		else
		{
			m_bounds = Bounds(
				m_transform->GetWorldPosition() + m_offset,
				m_size);

			m_isBoundsDirty = false;
		}
	}
}

void BoxCollider2D::CallOnCollisionEnter(const CollisionInfo& collisionInfo)
{
}

void BoxCollider2D::CallOnCollisionStay(const CollisionInfo& collisionInfo)
{
}

void BoxCollider2D::CallOnCollisionExit(const CollisionInfo& collisionInfo)
{
}

void BoxCollider2D::CallOnTriggerEnter(const CollisionInfo& collisionInfo)
{
}

void BoxCollider2D::CallOnTriggerStay(const CollisionInfo& collisionInfo)
{
}

void BoxCollider2D::CallOnTriggerExit(const CollisionInfo& collisionInfo)
{
}
