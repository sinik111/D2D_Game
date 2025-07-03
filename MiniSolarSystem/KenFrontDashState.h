#pragma once

#include "IState.h"

class KenFrontDashState :
	public IState
{
public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};