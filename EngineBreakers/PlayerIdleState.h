#pragma once

#include "PlayerStateBase.h"

class PlayerIdleState :
	public PlayerStateBase
{
private:
	float m_dummyGrabDistance = 200.0f;

	bool m_isWalkEnd = false;
	float m_walkEndTimer = 0.0f;
	float m_walkEndDuration = 0.25f;
	float m_walkDecSpeed = 0.0f;
	
	bool m_walkDecEnd = false;

	bool m_isDashEnd = false;
	float m_dashEndTimer = 0.0f;
	float m_dashEndDuration = 0.16f;
	float m_dashDecSpeed = 0.0f;

	bool m_dashDecEnd = false;

	


public:
	PlayerIdleState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;

	void AfterWalkDec(FSMContext& context);
	void AfterDashDec(FSMContext& context);
};