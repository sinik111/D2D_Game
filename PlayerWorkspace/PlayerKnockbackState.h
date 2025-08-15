#pragma once

#include "PlayerStateBase.h"

class PlayerKnockbackState :
	public PlayerStateBase
{
private:
	Vector2 m_knockbackDirection;
	Vector2 m_startPosition;
	Vector2 m_endPosition;

	float m_tempKnockbackTimer = 0.0f;
	float m_tempKnockbackDuration = 0.3f;
	float m_tempKnockbackDistance = 50.0f;

public:
	PlayerKnockbackState(Player* player);

public:
	bool CheckCondition(FSMContext& context) override;
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};