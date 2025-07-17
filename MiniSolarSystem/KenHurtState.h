#pragma once

#include "../D2DEngineLib/IState.h"

class KenHurtState :
    public IState
{
public:
	bool CheckCondition(FSMContext& context) override;
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};