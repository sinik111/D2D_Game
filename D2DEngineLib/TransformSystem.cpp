#include "pch.h"
#include "TransformSystem.h"

#include "Transform.h"
#include "ContainerUtility.h"

void TransformSystem::Register(Transform* transform)
{
	m_transforms.push_back(transform);
}

void TransformSystem::Unregister(Transform* transform)
{
	Util::OptimizedErase(m_transforms, transform);
}