#include "pch.h"
#include "GameObject.h"

#include "Component.h"

GameObject::GameObject(const std::wstring& name)
	: m_transform{ std::make_unique<Transform>() }, m_name{ name }
{

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
}

bool GameObject::IsDestroyed() const
{
	return m_isDestroyed;
}
