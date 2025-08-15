#pragma once
#include "EnemyState.h"

class EnemyEngage : public EnemyState
{
public:
	EnemyEngage() {}

	EnemyEngage(EnemyBase* pScript)
	{
		m_Script = pScript;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;	


};


