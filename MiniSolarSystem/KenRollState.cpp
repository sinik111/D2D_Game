#include "../D2DEngineLib/framework.h"
#include "KenRollState.h"

#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/MyTime.h"
#include "../D2DEngineLib/Transform.h"

#include "FSMContext.h"

void KenRollState::Enter(FSMContext& context)
{
	context.animator->Play(L"ken_roll");
}

void KenRollState::Update(FSMContext& context)
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

	context.transform->Translate(Vector2(horizontalInput, 0.0f) * m_speed * MyTime::DeltaTime());
}

void KenRollState::Exit(FSMContext& context)
{
}
