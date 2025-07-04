#include "../D2DEngineLib/framework.h"
#include "KenFrontDashState.h"

#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/MyTime.h"

#include "FSMContext.h"

void KenFrontDashState::Enter(FSMContext& context)
{
	context.animator->Play(L"ken_front_dash");
}

void KenFrontDashState::Update(FSMContext& context)
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

	if (horizontalInput < 0.0f)
	{
		context.shouldChangeState = true;
		context.nextStateName = L"BackDash";

		return;
	}

	if (horizontalInput == 0.0f)
	{
		context.shouldChangeState = true;
		context.nextStateName = L"Idle";

		return;
	}

	context.transform->Translate(Vector2::Right * m_speed * MyTime::DeltaTime());
}

void KenFrontDashState::Exit(FSMContext& context)
{
}
