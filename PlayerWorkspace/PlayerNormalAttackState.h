#pragma once

#include "PlayerStateBase.h"

class PlayerNormalAttackState :
	public PlayerStateBase
{
private:
	float m_dummyAttackAnimationDuration = 1.0f;
	float m_dummyAttackAnimationTimer = 0.0f;
	bool m_eventDone = false;

public:
	PlayerNormalAttackState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};