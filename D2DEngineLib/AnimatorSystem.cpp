#include "pch.h"
#include "AnimatorSystem.h"

#include "ContainerUtility.h"
#include "Animator.h"

void AnimatorSystem::Register(Animator* animator)
{
	m_animators.push_back(animator);
}

void AnimatorSystem::Unregister(Animator* animator)
{
	m_pendingUnregisteredAnimators.push_back(animator);
}

void AnimatorSystem::Update()
{
	for (size_t i = 0; i < m_animators.size(); ++i)
	{
		m_animators[i]->Update();
	}

	if (!m_pendingUnregisteredAnimators.empty())
	{
		Util::OptimizedErase(m_animators, m_pendingUnregisteredAnimators);

		m_pendingUnregisteredAnimators.clear();
	}
}