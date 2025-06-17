#include "pch.h"
#include "GameObject.h"

#include "Component.h"

GameObject::GameObject(const std::wstring& name)
	: m_transform{ std::make_unique<Transform>() },
	m_isDestroyed{ false },
	m_name{ name }
{

}

GameObject::~GameObject()
{

}

Transform* GameObject::GetTransform()
{
	return m_transform.get();
}

const std::wstring& GameObject::GetName()
{
	return m_name;
}

void GameObject::Destroy()
{
	m_isDestroyed = true;
}

bool GameObject::IsDestroyed()
{
	return m_isDestroyed;
}
