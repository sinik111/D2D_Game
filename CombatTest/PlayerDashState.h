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

	float m_dashStartTimer = 0.0f;
	float m_dashInDuration = 0.25f;
	float m_dashSpeed = 0.0f;
	bool m_dashDecEnd = false;

	float m_dashRealEndDuration = 0.1f;
	float m_dashRealEndTimer = 0.0f;
};