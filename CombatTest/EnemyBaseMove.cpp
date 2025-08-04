#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyBaseMove.h"

void EnemyBaseMove::Enter(FSMContext& context)
{	
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

