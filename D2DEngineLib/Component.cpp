#include "pch.h"
#include "Component.h"

#include "GameObject.h"

GameObject* Component::GetGameObject()
{
	return m_owner;
}

Transform* Component::GetTransform()
{
	return m_owner->GetTransform();
}

void Component::SetOwner(GameObject* gameObject)
{
	m_owner = gameObject;
}