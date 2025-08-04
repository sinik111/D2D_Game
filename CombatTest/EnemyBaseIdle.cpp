#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyBaseIdle.h"


void EnemyBaseIdle::Enter(FSMContext& context)
{	
	
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



