#pragma once
#include "EnemyState.h"

class EnemyHitAndRun : public EnemyState
{
public:
	EnemyHitAndRun() {}

	EnemyHitAndRun(EnemyBase* pScript)
	{
		m_Script = pScript;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;

	void HitAndRunAttack();

private:
	bool m_didAttack = false;
	bool m_didEvade = false;	
	
	Vector2 m_evadePos;

	float m_attackTime = 1.0f;
	float m_delayAfterEvade = 1.0f;
	

};

