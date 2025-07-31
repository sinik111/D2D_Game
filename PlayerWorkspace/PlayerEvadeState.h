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

public:
	PlayerEvadeState(Player* player);

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};