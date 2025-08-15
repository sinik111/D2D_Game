#pragma once
#include "EnemyState.h"

class EnemyKnockdown : public EnemyState
{
public:
	EnemyKnockdown() {}

	EnemyKnockdown(EnemyBase* pScript)
	{
		m_Script = pScript;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;

private:
	GameObject* m_grabIcon = nullptr;
	float m_KnockdownTimer = 0.0f;
	float m_knockdownDuration = 3.0f;
};

