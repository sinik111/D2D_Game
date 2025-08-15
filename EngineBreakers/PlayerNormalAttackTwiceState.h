#pragma once

#include "PlayerStateBase.h"
class PlayerNormalAttackTwiceState :
	public PlayerStateBase
{
private:
	float m_attackAnimationDuration = 0.8f;
	float m_attackAnimationTimer = 0.0f;
	bool m_eventDone = false;

	float m_moveDistSq = 0.0f;

public:
	PlayerNormalAttackTwiceState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;

	void CheckCancel(FSMContext& context);

	float m_comboStartRatio = 0.33f;
	float m_comboStartTime = 0.0f;
	float m_comboDuration = 1.0f;

};

