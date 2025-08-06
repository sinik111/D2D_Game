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
	bool m_didHit = false;
	bool m_doEvade = false;

	bool m_isEvadeStart = false;
	Vector2 m_evadePos;

	float m_delayAfterAttack = 1.0f;

};

