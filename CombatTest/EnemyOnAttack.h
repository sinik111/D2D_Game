#pragma once
#include "EnemyState.h"

class EnemyOnAttack : public EnemyState
{
public:
	EnemyOnAttack() {}

	EnemyOnAttack(EnemyBase* pScript)
	{
		m_Script = pScript;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;

private:

	float m_attackTime = 0.0f;
	float m_attackComboProb = 0.0f;
	bool m_didAttack = false;

};
