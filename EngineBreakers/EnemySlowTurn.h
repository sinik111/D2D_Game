#pragma once
#include "EnemyState.h"
class EnemySlowTurn : public EnemyState
{
public:
	
	EnemySlowTurn(EnemyBase* pScript = nullptr)
	{
		m_Script = pScript;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;


private:
	float m_slowTurnTimer = 0.0f;
	bool m_isTurnEnd = false;


};

