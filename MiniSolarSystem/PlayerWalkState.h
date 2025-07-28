#pragma once

#include "../D2DEngineLib/IState.h"

class PlayerWalkState :
	public IState
{
private:
	float m_dodgeAvailableTimer = 0.0f;
	bool m_canDodge = false;

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};