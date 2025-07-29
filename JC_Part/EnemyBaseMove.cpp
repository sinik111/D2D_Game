#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyBaseMove.h"

void EnemyBaseMove::Enter(FSMContext& context)
{	
	SetDirection(context);
}

void EnemyBaseMove::Update(FSMContext& context)
{
	SetDirection(context);
	
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


//void EnemyBaseMove::SetDirection(FSMContext& context)
//{
//	curDir = context.intParams[L"Direction"];
//
//	context.textRenderer->SetText(context.currentStateName + std::to_wstring(curDir));
//
//	if (curDir != prevDir)
//	{
//		prevDir = curDir;
//
//		context.animator->Play(animPath[curDir]);
//
//
//	}
//}