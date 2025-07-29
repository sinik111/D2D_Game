#pragma once

#include "FSMContext.h"

class IState
{
public:
	virtual ~IState() = default;

public:
	virtual bool CheckCondition(FSMContext& context)
	{
		return false;
	}

	virtual void Enter(FSMContext& context) = 0;
	virtual void Update(FSMContext& context) = 0;
	virtual void Exit(FSMContext& context) = 0;
};