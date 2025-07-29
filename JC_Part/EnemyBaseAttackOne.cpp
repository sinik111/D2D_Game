#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"

#include "EnemyBaseAttackOne.h"


void EnemyBaseAttackOne::Enter(FSMContext& context)
{
	

}

void EnemyBaseAttackOne::Update(FSMContext& context)
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

void EnemyBaseAttackOne::Exit(FSMContext& context)
{

}