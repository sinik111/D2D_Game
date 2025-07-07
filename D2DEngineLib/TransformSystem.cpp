#include "pch.h"
#include "TransformSystem.h"

#include "ContainerUtility.h"
#include "Transform.h"

void TransformSystem::Register(Transform* transform)
{
	m_transforms.push_back(transform);
}

void TransformSystem::Unregister(Transform* transform)
{
	Util::OptimizedErase(m_transforms, transform);
}

void TransformSystem::UnmarkDirtyThisFrame()
{
	for (auto& transform : m_transforms)
	{
		transform->UnmarkDirtyThisFrame();
	}
}