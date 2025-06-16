#include "pch.h"
#include "GameObject.h"

#include "Component.h"

GameObject::GameObject()
	: m_transform{ std::make_unique<Transform>() }
{
}

GameObject::~GameObject()
{
	for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
	{
		(*iter)->OnDestroy();
	}
}

Transform* GameObject::GetTransform()
{
	return m_transform.get();
}