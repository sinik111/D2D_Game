#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyBaseIdle.h"


void EnemyBaseIdle::Enter(FSMContext& context)
{	
	SetDirection(context);
	
}


void EnemyBaseIdle::Update(FSMContext& context)
{
	SetDirection(context);

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




//void EnemyBaseIdle::SetDirection(FSMContext& context)
//{
//	curDir = context.intParams[L"Direction"];
//	context.textRenderer->SetText(L"IdleState : " + std::to_wstring(curDir));
//	
//	if (curDir != prevDir)
//	{
//		prevDir = curDir;
//
//		switch (curDir)
//		{
//		case 0:
//			context.textRenderer->SetText(L"IdleState : NONE");
//			break;
//		case 5:
//			context.textRenderer->SetText(L"IdleState : RANDOM");
//			break;
//
//		case 8:
//			
//			context.animator->Play(L"EnemyBase_Move_UP");
//			break;
//
//		case 9:
//			
//			context.animator->Play(L"EnemyBase_Move_RIGHT_UP");
//			break;
//		case 6:
//			
//			context.animator->Play(L"EnemyBase_Move_RIGHT");
//			break;
//		case 3:
//			
//			context.animator->Play(L"EnemyBase_Move_RIGHT_DOWN");
//			break;
//		case 2:
//			
//			context.animator->Play(L"EnemyBase_Move_DOWN");
//			break;
//		case 1:
//		
//			context.animator->Play(L"EnemyBase_Move_LEFT_DOWN");
//			break;
//		case 4:
//			
//			context.animator->Play(L"EnemyBase_Move_LEFT");
//			break;
//		case 7:
//			
//			context.animator->Play(L"EnemyBase_Move_LEFT_UP");
//			break;
//		}
//	}
//}


