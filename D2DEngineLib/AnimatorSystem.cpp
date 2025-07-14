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
	Util::OptimizedErase(m_animators, animator);
}

void AnimatorSystem::Update()
{
	for (const auto& animator : m_animators)
	{
		animator->Update();
	}
}