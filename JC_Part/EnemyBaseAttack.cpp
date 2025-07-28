#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"

#include "EnemyBaseAttack.h"


void EnemyBaseAttack::Enter(FSMContext& context)
{
	switch (context.intParams[L"Direction"])
	{
	case 0:
		break;
	case 5:
		break;

	case 8:
		context.animator->Play(L"EnemyBase_Attack_UP");
		break;

	case 9:
		context.animator->Play(L"EnemyBase_Attack_RIGHT_UP");
		break;
	case 6:
		context.animator->Play(L"EnemyBase_Attack_RIGHT");
		break;
	case 3:
		context.animator->Play(L"EnemyBase_Attack_RIGHT_DOWN");
		break;
	case 2:
		context.animator->Play(L"EnemyBase_Attack_DOWN");
		break;
	case 1:
		context.animator->Play(L"EnemyBase_Attack_LEFT_DOWN");
		break;
	case 4:
		context.animator->Play(L"EnemyBase_Attack_LEFT");
		break;
	case 7:
		context.animator->Play(L"EnemyBase_Attack_LEFT_UP");
		break;
	}

}

void EnemyBaseAttack::Update(FSMContext& context)
{
	if (context.animator->IsFinished())
	{
		context.shouldChangeState = true;
		
		if (context.boolParams[L"isMove"])
		{
			context.nextStateName = L"Move";
		}
		else
		{
			context.nextStateName = L"Idle";
		}

		return;
	}
}

void EnemyBaseAttack::Exit(FSMContext& context)
{
}