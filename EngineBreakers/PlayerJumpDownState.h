#pragma once
#include "../D2DEngineLib/IState.h"
#include "PlayerStateBase.h"
class PlayerJumpDownState
	: public PlayerStateBase
{
	float m_jumpDownTimer = 0.0f;
	float m_jumpDownDurtion = 2.084f;
	//float m_elapsedTime
public:
	PlayerJumpDownState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};

