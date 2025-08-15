#pragma once

#include "../D2DEngineLib/IState.h"

#include "PlayerStateBase.h"

class PlayerEvadeState :
	public PlayerStateBase
{
private:
	Vector2 m_dodgeDirection;
	Vector2 m_startPosition;
	Vector2 m_endPosition;

	float m_dashTransitDuration = 0.3f;
	float m_dashTransitTimer = 0.0f;

	float m_evadeStartTimer = 0.0f;
	float m_evadeInDuration = 0.25f;
	float m_evadeSpeed = 0.0f;
	bool m_evadeAccEnd = false;


public:
	PlayerEvadeState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;

	void CheckCancel(FSMContext& context);
};