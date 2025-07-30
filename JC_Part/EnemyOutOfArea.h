#pragma once
#include "EnemyState.h"

class EnemyOutOfArea : public EnemyState
{
public:
	EnemyOutOfArea() {}

	EnemyOutOfArea(EnemyBase* pScript)
	{
		m_Script = pScript;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;

};

