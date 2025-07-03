#pragma once

#include "IState.h"

class KenRollState :
	public IState
{
public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};