#include "pch.h"
#include "Component.h"

Component::Component()
	: m_owner{ nullptr }
{
}

void Component::OnEnable()
{
}

void Component::OnDestroy()
{
}

void Component::SetOwner(GameObject* gameObject)
{
	m_owner = gameObject;
}
