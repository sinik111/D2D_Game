#pragma once

#include "EnemyState.h"

class EnemyOnAttackThird : public EnemyState
{
public:

	EnemyOnAttackThird(EnemyBase* pScript = nullptr)
	{
		m_Script = pScript;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;


private:

	float m_attackTime = 2.0f;	
	float m_didAttack = false;

	float m_comboDelay = 0.3f;
};
