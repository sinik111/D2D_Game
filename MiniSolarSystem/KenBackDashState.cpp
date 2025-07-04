#include "../D2DEngineLib/framework.h"
#include "KenBackDashState.h"

#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/MyTime.h"
#include "../D2DEngineLib/Transform.h"

#include "FSMContext.h"

void KenBackDashState::Enter(FSMContext& context)
{
	context.animator->Play(L"ken_back_dash");
	context.currentStateName = L"BackDash";
}

void KenBackDashState::Update(FSMContext& context)
{
	if (context.triggerParams[L"SpinningKick"])
	{
		context.shouldChangeState = true;
		context.nextStateName = L"SpinningKick";

		return;
	}

	if (context.triggerParams[L"Roll"])
	{
		context.shouldChangeState = true;
		context.nextStateName = L"Roll";

		return;
	}

	float horizontalInput = context.floatParams[L"HorizontalInput"];

	if (horizontalInput > 0.0f)
	{
		context.shouldChangeState = true;
		context.nextStateName = L"FrontDash";

		return;
	}

	if (horizontalInput == 0.0f)
	{
		context.shouldChangeState = true;
		context.nextStateName = L"Idle";

		return;
	}
	
	context.transform->Translate(Vector2::Left * m_speed * MyTime::DeltaTime());
}

void KenBackDashState::Exit(FSMContext& context)
{
}
