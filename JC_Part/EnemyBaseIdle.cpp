#include "EnemyBaseIdle.h"

#include "../D2DEngineLib/framework.h"
#include "KenIdleState.h"

#include "../D2DEngineLib/Animator.h"
//#include <cmath>


void EnemyBaseIdle::Enter(FSMContext& context)
{
	switch (context.intParams[L"Direction"])
	{
	case 0:
		break;
	case 5:
		break;

	case 8:
		context.animator->Play(L"EnemyBase_IDLE_UP");
		break;

	case 9:
		context.animator->Play(L"EnemyBase_IDLE_RIGHT_UP");
		break;
	case 6:
		context.animator->Play(L"EnemyBase_IDLE_RIGHT");
		break;
	case 3:
		context.animator->Play(L"EnemyBase_IDLE_RIGHT_DOWN");
		break;
	case 2:
		context.animator->Play(L"EnemyBase_IDLE_DOWN");
		break;
	case 1:
		context.animator->Play(L"EnemyBase_IDLE_LEFT_DOWN");
		break;
	case 4:
		context.animator->Play(L"EnemyBase_IDLE_LEFT");
		break;
	case 7:
		context.animator->Play(L"EnemyBase_IDLE_LEFT_UP");
		break;
	}	
}


void EnemyBaseIdle::Update(FSMContext& context)
{
	if (context.boolParams[L"isMove"])
	{
		context.shouldChangeState = true;
		context.nextStateName = L"Move";
		
		return;
	}
}

void EnemyBaseIdle::Exit(FSMContext& context)
{
}


