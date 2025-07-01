#include "pch.h"
#include "Component.h"

#include "GameObject.h"
#include "ComponentSystem.h"

Component::Component()
{
	ComponentSystem::Get().RegisterValidCheck(this);
}

Component::~Component()
{
	ComponentSystem::Get().UnregisterValidCheck(this);
}

GameObject* Component::GetGameObject() const
{
	return m_owner;
}

Transform* Component::GetTransform() const
{
	return m_owner->GetTransform();
}

bool Component::IsValid(Component* component)
{
	if (component == nullptr)
	{
		return false;
	}

	return ComponentSystem::Get().IsValid(component);
}

void Component::SetOwner(GameObject* gameObject)
{
	m_owner = gameObject;
}