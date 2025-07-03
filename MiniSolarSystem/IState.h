#pragma once

struct FSMContext;

class IState
{
public:
	virtual void Enter(FSMContext& context) = 0;
	virtual void Update(FSMContext& context) = 0;
	virtual void Exit(FSMContext& context) = 0;
};