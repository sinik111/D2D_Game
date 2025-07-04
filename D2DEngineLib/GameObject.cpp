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
			CallOnDestroy(script);
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

void GameObject::Update()
{
	for (auto iter = m_components.begin(); iter != m_components.end();)
	{
		if ((*iter)->m_isDestroyed)
		{
			if (Script* script = dynamic_cast<Script*>((*iter).get()))
			{
				CallOnDestroy(script);
			}

			iter = m_components.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void GameObject::Destroy()
{
	m_isDestroyed = true;

	for (const auto& child : m_transform->GetChildren())
	{
		child->GetGameObject()->Destroy();
	}
}

void GameObject::CallOnDestroy(Script* script)
{
	script->OnDestroy();
}

GameObject* GameObject::Find(const std::wstring name)
{
	return SceneManager::Get().Find(name);
}