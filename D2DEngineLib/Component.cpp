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

bool Component::IsEnabled() const
{
	return m_isEnabled;
}

bool Component::IsActiveAndEnabled() const
{
	return m_isEnabled && m_owner->IsActive();
}

void Component::SetEnabled(bool enabled)
{
	m_isEnabled = enabled;
	if (m_owner->IsActive())
	{
		if (enabled)
		{
			RegisterToSystem();
		}
		else
		{
			UnregisterFromSystem();
		}
	}
}

void Component::Initialize()
{

}

void Component::RegisterToSystem()
{

}

void Component::UnregisterFromSystem()
{

}

void Component::OnDestroy()
{

}

void Component::Destroy()
{
	assert(dynamic_cast<Transform*>(this) == nullptr && L"Transform 컴포넌트는 삭제할 수 없습니다");

	m_isDestroyed = true;
}

void Component::SetOwner(GameObject* gameObject)
{
	m_owner = gameObject;
}