#include "pch.h"
#include "GameObject.h"

#include "Component.h"
#include "SceneManager.h"
#include "Script.h"

GameObject::GameObject(const std::wstring& name)
	: m_transform{ std::make_unique<Transform>() }, m_name{ name }
{
	((Component*)m_transform.get())->SetOwner(this);
}

GameObject::~GameObject()
{
	for (const auto& comp : m_components)
	{
		if (Script* script = dynamic_cast<Script*>(comp.get()))
		{
			script->OnDestroy();
		}
	}
}

Transform* GameObject::GetTransform() const
{
	return m_transform.get();
}

const std::wstring& GameObject::GetName() const
{
	return m_name;
}

void GameObject::Destroy()
{
	m_isDestroyed = true;

	for (const auto& child : m_transform->GetChildren())
	{
		child->GetGameObject()->Destroy();
	}
}

bool GameObject::IsDestroyed() const
{
	return m_isDestroyed;
}

GameObject* GameObject::Find(const std::wstring name)
{
	return SceneManager::Get().Find(name);
}

bool GameObject::IsValid(GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return false;
	}

	return SceneManager::Get().IsValid(gameObject);
}
