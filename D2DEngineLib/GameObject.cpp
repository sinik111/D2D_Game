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
	for (const auto& component : m_components)
	{
		component->UnregisterFromSystem();

		if (Script* script = dynamic_cast<Script*>(component.get()))
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

void GameObject::CleanupDestroyedComponents()
{
	for (size_t i = 0; i < m_components.size(); )
	{
		if (m_components[i]->m_isDestroyed)
		{
			m_components[i]->UnregisterFromSystem();

			std::swap(m_components[i], m_components.back());

			m_components.pop_back();

			continue;
		}

		++i;
	}

	m_hasDestroyedComponent = false;
}

void GameObject::Destroy()
{
	m_isDestroyed = true;

	for (const auto& child : m_transform->GetChildren())
	{
		child->GetGameObject()->Destroy();
	}
}

bool GameObject::HasAddedComponent() const
{
	return !m_addedComponents.empty();
}

bool GameObject::HasDestroyedComponents() const
{
	return m_hasDestroyedComponent;
}

void GameObject::Initialize()
{
	if (!m_isInitialized)
	{
		m_isInitialized = true;

		size_t initialSize = m_components.size();
		for (size_t i = 0; i < initialSize; ++i)
		{
			m_components[i]->Initialize();
			m_components[i]->RegisterToSystem();
		}
	}
}

void GameObject::InitializeAddedComponents()
{
	for (const auto& component : m_addedComponents)
	{
		component->Initialize();
		component->RegisterToSystem();
	}

	m_addedComponents.clear();
}

void GameObject::MarkComponentDestroyed()
{
	m_hasDestroyedComponent = true;
}

GameObject* GameObject::Find(const std::wstring name)
{
	return SceneManager::Get().Find(name);
}