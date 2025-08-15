#pragma once
#include "EnemyState.h"

class EnemyDead : public EnemyState
{
public:
	EnemyDead(EnemyBase* pScript = nullptr)
	{
		m_Script = pScript;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;

private:
	float m_timeForHAIKU = 3.0f;

};

