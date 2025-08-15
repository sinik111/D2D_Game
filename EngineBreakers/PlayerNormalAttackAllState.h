#pragma once

#include "PlayerStateBase.h"
class PlayerNormalAttackAllState :
	public PlayerStateBase
{

private:
	float m_attackAnimationDuration = 0.8f;
	float m_attackAnimationTimer = 0.0f;
	bool m_eventDone = false;

public:
	PlayerNormalAttackAllState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;

	void CheckCancel(FSMContext& context);
};

