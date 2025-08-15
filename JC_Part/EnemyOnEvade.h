#pragma once
#include "EnemyState.h"

class EnemyOnEvade : public EnemyState
{

public:
	EnemyOnEvade() {}

	EnemyOnEvade(EnemyBase* pScript)
	{
		m_Script = pScript;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;

	
private:
	bool m_isEvadeStart = false;
	Vector2 m_evadePos;
};

