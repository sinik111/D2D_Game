#pragma once

#include "PlayerStateBase.h"

class PlayerWalkState :
	public PlayerStateBase
{
private:
	bool m_canDodge = false;

public:
	PlayerWalkState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};