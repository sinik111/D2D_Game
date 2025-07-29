#pragma once
#include "EnemyState.h"

class EnemyBaseIdle :
	public EnemyState
{
public:

	EnemyBaseIdle()
	{
		animPath[0] = L"EnemyBase_Move_UP";
		animPath[1] = L"EnemyBase_Move_LEFT_DOWN";
		animPath[2] = L"EnemyBase_Move_DOWN";
		animPath[3] = L"EnemyBase_Move_RIGHT_DOWN";
		animPath[4] = L"EnemyBase_Move_LEFT";
		animPath[5] = L"EnemyBase_Move_UP";
		animPath[6] = L"EnemyBase_Move_RIGHT";
		animPath[7] = L"EnemyBase_Move_LEFT_UP";
		animPath[8] = L"EnemyBase_Move_UP";
		animPath[9] = L"EnemyBase_Move_RIGHT_UP";
	}
	void Enter(FSMContext& context) override;
	void Update(FSMContext& context) override;
	void Exit(FSMContext& context) override;

	//void SetDirection(FSMContext& context);

	

};