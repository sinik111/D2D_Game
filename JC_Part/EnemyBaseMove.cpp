#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"

#include "EnemyBaseMove.h"

void EnemyBaseMove::Enter(FSMContext& context)
{
	switch (context.intParams[L"Direction"])
	{
	case 0:
		break;
	case 5:
		break;

	case 8:
		context.animator->Play(L"EnemyBase_Move_UP");
		break;

	case 9:
		context.animator->Play(L"EnemyBase_Move_RIGHT_UP");
		break;
	case 6:
		context.animator->Play(L"EnemyBase_Move_RIGHT");
		break;
	case 3:
		context.animator->Play(L"EnemyBase_Move_RIGHT_DOWN");
		break;
	case 2:
		context.animator->Play(L"EnemyBase_Move_DOWN");
		break;
	case 1:
		context.animator->Play(L"EnemyBase_Move_LEFT_DOWN");
		break;
	case 4:
		context.animator->Play(L"EnemyBase_Move_LEFT");
		break;
	case 7:
		context.animator->Play(L"EnemyBase_Move_LEFT_UP");
		break;
	}

}

void EnemyBaseMove::Update(FSMContext& context)
{
	if (!context.boolParams[L"isMove"])
	{
		context.shouldChangeState = true;
		context.nextStateName = L"Idle";

		return;
	}
}

void EnemyBaseMove::Exit(FSMContext& context)
{
	
}