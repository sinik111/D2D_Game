#pragma once

#include "IState.h"

class KenFrontDashState :
	public IState
{
private:
	float m_speed = 100.0f;

public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};