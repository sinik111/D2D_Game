#pragma once

#include "PlayerStateBase.h"

class PlayerGrabState :
	public PlayerStateBase
{
private:
	float m_dummyGrabAnimationDuration = 1.5f;
	float m_dummyGrabAnimationTimer = 0.0f;
public:
	PlayerGrabState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};