#include "../D2DEngineLib/framework.h"
#include "KenSpinningKickState.h"

#include "../D2DEngineLib/Animator.h"

#include "FSMContext.h"

void KenSpinningKickState::Enter(FSMContext& context)
{
	context.animator->Play(L"ken_spinning_kick");
}

void KenSpinningKickState::Update(FSMContext& context)
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

void KenSpinningKickState::Exit(FSMContext& context)
{
}
