#include "../D2DEngineLib/framework.h"
#include "KenHurtState.h"

#include "../D2DEngineLib/Animator.h"

#include "FSMContext.h"

bool KenHurtState::CheckCondition(FSMContext& context)
{
	if (context.triggerParams[L"Hurt"])
	{
		return true;
	}

	return false;
}

void KenHurtState::Enter(FSMContext& context)
{
	context.animator->Play(L"ken_hurt");
	context.currentStateName = L"Hurt";
}

void KenHurtState::Update(FSMContext& context)
{
	if (context.animator->IsFinished())
	{
		context.shouldChangeState = true;

		float horizontalInput = context.floatParams[L"HorizontalInput"];

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

void KenHurtState::Exit(FSMContext& context)
{
}
