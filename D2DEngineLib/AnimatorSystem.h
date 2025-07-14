#pragma once

class Animator;

class AnimatorSystem
{
private:
	std::vector<Animator*> m_animators;

public:
	void Register(Animator* animator);
	void Unregister(Animator* animator);

public:
	void Update();
};