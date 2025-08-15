#pragma once
#include "EnemyState.h"

class EnemyGrabbed : public EnemyState
{
public:	

	EnemyGrabbed(EnemyBase* pScript = nullptr)
	{
		m_Script = pScript;
	}

	void Enter(FSMContext& context) override;

	void Update(FSMContext& context) override;

	void Exit(FSMContext& context) override;

private:
	float m_grabTimer = 3.0f;

};

