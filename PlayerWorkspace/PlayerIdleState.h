#pragma once

#include "PlayerStateBase.h"

class PlayerIdleState :
	public PlayerStateBase
{
private:
	float m_dummyGrabDistance = 200.0f;

public:
	PlayerIdleState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};