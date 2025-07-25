#include "../D2DEngineLib/framework.h"
#include "KenFrontDashState.h"

#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/MyTime.h"
#include "../D2DEngineLib/Transform.h"
#include "../D2DEngineLib/RigidBody2D.h"

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

	if (context.triggerParams[L"Roll"] && context.boolParams[L"IsGround"])
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

	Vector2 velocity = context.rigidBody2d->GetVelocity();

	context.rigidBody2d->SetVelocity(Vector2(horizontalInput * 100.0f, velocity.y));
}

void KenFrontDashState::Exit(FSMContext& context)
{
	Vector2 velocity = context.rigidBody2d->GetVelocity();

	context.rigidBody2d->SetVelocity(Vector2(0.0f, velocity.y));
}
