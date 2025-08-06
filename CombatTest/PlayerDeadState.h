#pragma once

#include "PlayerStateBase.h"

class PlayerDeadState :
	public PlayerStateBase
{
public:
	PlayerDeadState(Player* player);

public:
	bool CheckCondition(FSMContext& context) override;
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};