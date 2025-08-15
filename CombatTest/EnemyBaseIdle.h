#pragma once
#include "EnemyState.h"

class EnemyBaseIdle :
	public EnemyState
{
public:
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;
};