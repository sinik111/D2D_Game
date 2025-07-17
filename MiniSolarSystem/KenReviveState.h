#pragma once

#include "../D2DEngineLib/IState.h"

class KenReviveState :
    public IState
{
public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};

