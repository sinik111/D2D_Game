#pragma once
#include "EnemyState.h"

class EnemyEvadeAndRun : public EnemyState
{
public:

	EnemyEvadeAndRun(EnemyBase* pScript = nullptr)
	{
		m_Script = pScript;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;

private:
	bool m_didEvade = false;	

	float m_delayAfterEvade = 1.0f;
};

