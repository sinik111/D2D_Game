#pragma once

#include "PlayerStateBase.h"

class PlayerWalkState :
	public PlayerStateBase
{
private:
	float m_dummyGrabDistance = 200.0f;
	//bool m_canDodge = false;

	float m_walkStartTimer = 0.0f;
	float m_walkInDuration = 0.25f;
	float m_walkSpeed = 0.0f;
	bool m_walkAccEnd = false;

	bool m_isDashEnd = false;
	float m_dashEndTimer = 0.0f;
	float m_dashEndDuration = 0.16f;
	float m_dashDecSpeed = 0.0f;

	bool m_dashDecEnd = false;
		

public:
	PlayerWalkState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;

	void AfterDashDec(FSMContext& context);
	void StartWalkAcc(FSMContext& context);
};