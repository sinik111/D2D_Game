#pragma once

#include "PlayerStateBase.h"

class PlayerHeavyAttackState :
	public PlayerStateBase
{
private:
	float m_dummyAttackAnimationDuration = 1.5f;
	float m_dummyAttackAnimationTimer = 0.0f;
	float m_secondAttackTimer = 0.0f;
	float m_secondAttackCheckTime = 1.0f;
	bool m_eventDone = false;
	bool m_isSecondAttack = false;
	bool m_isStoppedPressing = false;
	bool m_doSecondAttack = false;

public:
	PlayerHeavyAttackState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};

