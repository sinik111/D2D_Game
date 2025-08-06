#pragma once

#include "PlayerStateBase.h"

class PlayerWalkState :
	public PlayerStateBase
{
private:
	float m_dummyGrabDistance = 200.0f;
	bool m_canDodge = false;

public:
	PlayerWalkState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};