#pragma once

#include "IState.h"

class KenDeadState :
    public IState
{
private:
	bool m_once = false;

public:
	bool CheckCondition(FSMContext& context) override;
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};