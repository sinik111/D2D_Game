#pragma once

#include "PlayerStateBase.h"

class PlayerKnockdownState :
	public PlayerStateBase
{
private:
	Vector2 m_knockbackDirection;
	Vector2 m_startPosition;
	Vector2 m_endPosition;

	float m_knockdownTimer = 0.0f;
	float m_knockdownDuration = 2.0f;	

	bool m_knockbackDone = false;

public:
	PlayerKnockdownState(Player* player);

public:
	bool CheckCondition(FSMContext& context) override;
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};