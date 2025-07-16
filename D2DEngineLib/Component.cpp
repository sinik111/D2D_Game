#include "pch.h"
#include "Component.h"

#include "GameObject.h"

GameObject* Component::GetGameObject() const
{
	return m_owner;
}

Transform* Component::GetTransform() const
{
	return m_owner->GetTransform();
}

void Component::Destroy(Object* object, float delay)
{
	object->Object::Destroy(object, delay);
}

void Component::RegisterToSystem()
{
	
}

void Component::UnregisterFromSystem()
{
	
}

void Component::Initialize()
{
	
}

void Component::Destroy()
{
	assert(dynamic_cast<Transform*>(this) == nullptr && L"Transform ������Ʈ�� ������ �� �����ϴ�");

	m_isDestroyed = true;

	m_owner->MarkComponentDestroyed();
}

void Component::SetOwner(GameObject* gameObject)
{
	m_owner = gameObject;
}