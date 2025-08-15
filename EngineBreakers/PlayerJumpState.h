#pragma once

#include "../D2DEngineLib/IState.h"

#include "PlayerStateBase.h"
class PlayerJumpState :
	public PlayerStateBase
{
	float m_jumpUpTimer = 0.0f;
	float m_jumpUpDuration = 2.0f;
	float m_walkTimer = 0.0f;
	float m_walkDuration = 1.5f;

public:
	PlayerJumpState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};

