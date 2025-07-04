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

void Component::Destroy(Object* object)
{
	object->Destroy();
}

void Component::Destroy()
{
	assert(dynamic_cast<Transform*>(this) == nullptr && L"Transform ������Ʈ�� ������ �� �����ϴ�");

	m_isDestroyed = true;
}

void Component::SetOwner(GameObject* gameObject)
{
	m_owner = gameObject;
}