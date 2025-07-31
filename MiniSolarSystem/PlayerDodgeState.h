#pragma once

#include "../D2DEngineLib/IState.h"

class PlayerEvadeState :
	public IState
{
private:
	Vector2 m_dodgeDirection;
	Vector2 m_startPosition;
	Vector2 m_endPosition;

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};