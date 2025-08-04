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
	
	void SetRoam(FSMContext& context);

	bool SightCheck(const Vector2& playerPos, const Vector2& enemyPos);

	void PlayerSearch(FSMContext& context);

private:
	float m_roamWaitTimer = 0.0f;
	bool m_isWaitingToRoam = false;
	bool m_isRoaming = false;
};


