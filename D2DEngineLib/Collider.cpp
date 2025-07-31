#include "pch.h"
#include "Collider.h"

#include "GameObject.h"
#include "RigidBody2D.h"
#include "Script.h"
#include "ComponentSystem.h"

void Collider::Initialize()
{
	m_transform = GetTransform();
	m_rigidBody2d = GetGameObject()->GetComponent<RigidBody2D>();

	Update();
}

void Collider::RegisterToSystem()
{
	ComponentSystem::Get().Physics().RegisterCollider(this);
}

void Collider::UnregisterFromSystem()
{
	ComponentSystem::Get().Physics().UnregisterCollider(this);
}

void Collider::RegisterScript(Script* script)
{
	m_scriptsForCallBack.push_back(script);
}

void Collider::UnregisterScript(Script* script)
{
	Util::OptimizedErase(m_scriptsForCallBack, script);
}

RigidBody2D* Collider::GetRigidBody2D() const
{
	return m_rigidBody2d;
}

void Collider::SetTrigger(bool trigger)
{
	m_isTrigger = trigger;
}

void Collider::SetOffset(const Vector2& offset)
{
	m_offset = offset;

	m_isColliderDirty = true;
}

void Collider::SetLayer(CollisionLayer layer)
{
	m_layer = layer;
}

void Collider::SetIsCollide(bool isCollide)
{
	m_isCollide = isCollide;
}

void Collider::AddBelongingNode(QuadtreeNode* node)
{
	m_belongingNodes.push_back(node);
}

bool Collider::GetTrigger() const
{
	return m_isTrigger;
}

Bounds Collider::GetSpatialBounds() const
{
	return m_spatialBounds;
}

CollisionLayer Collider::GetLayer() const
{
	return m_layer;
}

std::vector<QuadtreeNode*>& Collider::GetBelongingNode()
{
	return m_belongingNodes;
}

Vector2 Collider::GetOffset() const
{
	return m_offset;
}

void Collider::Update()
{
	if (m_rigidBody2d == nullptr)
	{
		if (m_isColliderDirty || m_transform->IsDirtyThisFrame())
		{
			UpdateCollider();

			CalculateSpatialBounds();
		}
	}
	else
	{
		UpdateCollider();

		CalculateSpatialBounds();
	}
}

void Collider::Render()
{

}

void Collider::CallOnCollisionEnter(const Collision& collision)
{
	for (const auto& script : m_scriptsForCallBack)
	{
		script->OnCollisionEnter(collision);
	}
}
void Collider::CallOnCollisionStay(const Collision& collision)
{
	for (const auto& script : m_scriptsForCallBack)
	{
		script->OnCollisionStay(collision);
	}
}
void Collider::CallOnCollisionExit(const Collision& collision)
{
	for (const auto& script : m_scriptsForCallBack)
	{
		script->OnCollisionExit(collision);
	}
}
void Collider::CallOnTriggerEnter(const Collision& collision)
{
	for (const auto& script : m_scriptsForCallBack)
	{
		script->OnTriggerEnter(collision);
	}
}
void Collider::CallOnTriggerStay(const Collision& collision)
{
	for (const auto& script : m_scriptsForCallBack)
	{
		script->OnTriggerStay(collision);
	}
}
void Collider::CallOnTriggerExit(const Collision& collision)
{
	for (const auto& script : m_scriptsForCallBack)
	{
		script->OnTriggerExit(collision);
	}
}
