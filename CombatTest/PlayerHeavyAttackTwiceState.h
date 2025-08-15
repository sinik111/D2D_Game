#pragma once

#include "PlayerStateBase.h"

class PlayerHeavyAttackTwiceState :
	public PlayerStateBase
{

private:
	float m_attackAnimationDuration = 1.0f;
	float m_attackAnimationTimer = 0.0f;
	bool m_eventDone = false;

	float m_secondAttackTimer = 0.0f;
	float m_secondAttackCheckTime = 1.0f;

	bool m_isSecondAttack = false;
	bool m_isStoppedPressing = false;
	bool m_doSecondAttack = false;

public:
	PlayerHeavyAttackTwiceState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;

	void CheckCancel(FSMContext& context);
};

