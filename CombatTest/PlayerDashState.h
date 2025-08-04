#pragma once

#include "PlayerStateBase.h"

class PlayerDashState :
	public PlayerStateBase
{
public:
	PlayerDashState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};