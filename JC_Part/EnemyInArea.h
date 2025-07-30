#pragma once
#include "EnemyState.h"

class EnemyInArea : public EnemyState
{
public:
	EnemyInArea() {}

	EnemyInArea(EnemyBase* pScript)
	{
		m_Script = pScript;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;
};


