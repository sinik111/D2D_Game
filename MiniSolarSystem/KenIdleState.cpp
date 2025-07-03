#include "../D2DEngineLib/framework.h"
#include "KenIdleState.h"

#include "../D2DEngineLib/Animator.h"

#include "FSMContext.h"

void KenIdleState::Enter(FSMContext& context)
{
	context.animator->Play(L"ken_idle");
}

void KenIdleState::Update(FSMContext& context)
{
	if (context.triggerParams[L"SpinningKick"])
	{
		context.hasRequest = true;
		context.nextStateName = L"SpinningKick";

		return;
	}

	if (context.triggerParams[L"Roll"])
	{
		context.hasRequest = true;
		context.nextStateName = L"Roll";

		return;
	}

	float horizontalInput = context.floatParams[L"HorizontalInput"];

	if (horizontalInput < 0.0f)
	{
		context.hasRequest = true;
		context.nextStateName = L"BackDash";

		return;
	}

	if (horizontalInput > 0.0f)
	{
		context.hasRequest = true;
		context.nextStateName = L"FrontDash";

		return;
	}
}

void KenIdleState::Exit(FSMContext& context)
{
}
