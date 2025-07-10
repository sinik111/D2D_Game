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
		comp->UnregisterFromSystem();
		if (Script* script = dynamic_cast<Script*>(comp.get()))
		{
			if (script->IsActiveAndEnabled())
			{
				script->OnDisable();
			}

			CallComponentsOnDestroy(script);
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

bool GameObject::IsActive() const
{
	return m_isActiveSelf && m_isActiveInHierarchy;
}

bool GameObject::IsActiveSelf() const
{
	return m_isActiveSelf;
}

void GameObject::SetActive(bool active)
{
	if (m_isActiveSelf != active)
	{
		m_isActiveSelf = active;

		if (m_isActiveInHierarchy)
		{
			auto& children = m_transform->GetChildren();
			for (auto& child : children)
			{
				child->GetGameObject()->SetActiveInHierarchy(active);
			}

			if (m_isInitialized)
			{
				if (active)
				{
					RegisterComponentsToSystem();
				}
				else
				{
					UnregisterComponentsFromSystem();
				}
			}
		}
	}
}

void GameObject::SetActiveInHierarchy(bool activeInHierarchy)
{
	if (m_isActiveInHierarchy != activeInHierarchy)
	{
		m_isActiveInHierarchy = activeInHierarchy;

		if (m_isActiveSelf)
		{
			auto& children = m_transform->GetChildren();
			for (auto& child : children)
			{
				child->GetGameObject()->SetActiveInHierarchy(activeInHierarchy);
			}

			if (m_isInitialized)
			{
				if (activeInHierarchy)
				{
					RegisterComponentsToSystem();
				}
				else
				{
					UnregisterComponentsFromSystem();
				}
			}
		}
	}
}

void GameObject::Initialize()
{
	if (!m_isInitialized)
	{
		CallComponentsInitialize();
		RegisterComponentsToSystem();

		m_isInitialized = true;
	}
}

void GameObject::CleanupDestroyedComponents()
{
	for (size_t i = 0; i < m_components.size(); )
	{
		if (m_components[i]->m_isDestroyed)
		{
			if (Script* script = dynamic_cast<Script*>(m_components[i].get()))
			{
				CallComponentsOnDestroy(script);
			}

			std::swap(m_components[i], m_components.back());

			m_components.pop_back();

			continue;
		}

		++i;
	}
}

void GameObject::CallComponentsInitialize()
{
	for (size_t i = 0; i < m_components.size(); ++i)
	{
		m_components[i]->Initialize();
	}
}

void GameObject::RegisterComponentsToSystem()
{
	for (auto& component : m_components)
	{
		if (component->IsEnabled())
		{
			component->RegisterToSystem();
		}
	}
}

void GameObject::UnregisterComponentsFromSystem()
{
	for (auto& component : m_components)
	{
		if (component->IsEnabled())
		{
			component->UnregisterFromSystem();
		}
	}
}

void GameObject::CallComponentsOnDestroy(Script* script)
{
	script->OnDestroy();
}

void GameObject::Destroy()
{
	m_isDestroyed = true;

	for (const auto& child : m_transform->GetChildren())
	{
		child->GetGameObject()->Destroy();
	}
}

GameObject* GameObject::Find(const std::wstring name)
{
	return SceneManager::Get().Find(name);
}