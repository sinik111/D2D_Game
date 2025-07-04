#include "../D2DEngineLib/framework.h"
#include "KenReviveState.h"

#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/MyTime.h"
#include "../D2DEngineLib/Transform.h"

#include "FSMContext.h"

void KenReviveState::Enter(FSMContext& context)
{
	context.animator->Play(L"ken_revive");
	context.currentStateName = L"Revive";
}

void KenReviveState::Update(FSMContext& context)
{
	float horizontalInput = context.floatParams[L"HorizontalInput"];

	if (context.animator->IsFinished())
	{
		context.shouldChangeState = true;

		if (horizontalInput < 0.0f)
		{
			context.nextStateName = L"BackDash";
		}
		else if (horizontalInput > 0.0f)
		{
			context.nextStateName = L"FrontDash";
		}
		else
		{
			context.nextStateName = L"Idle";
		}

		return;
	}
}

void KenReviveState::Exit(FSMContext& context)
{
}