#include "pch.h"
#include "GameObject.h"

#include "Transform.h"

GameObject::GameObject()
	: m_transform{ new Transform() }
{
}

GameObject::~GameObject()
{
	delete m_transform;
}

Transform* GameObject::GetTransform()
{
	return m_transform;
}