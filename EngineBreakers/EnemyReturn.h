#pragma once
#include "EnemyState.h"

class EnemyReturn : public EnemyState
{
public:
	EnemyReturn() {}

	EnemyReturn(EnemyBase* pScript)
	{
		m_Script = pScript;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;
};



