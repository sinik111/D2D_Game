#include "../D2DEngineLib/framework.h"
#include "KenRollState.h"

#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/MyTime.h"
#include "../D2DEngineLib/Transform.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "FSMContext.h"

void KenRollState::Enter(FSMContext& context)
{
	context.animator->Play(L"ken_roll");

	context.rigidBody2d->AddForce(Vector2(0.0f, 15000.0f));
}

void KenRollState::Update(FSMContext& context)
{
	float horizontalInput = context.floatParams[L"HorizontalInput"];

	if (context.boolParams[L"IsGround"])
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

	Vector2 velocity = context.rigidBody2d->GetVelocity();

	context.rigidBody2d->SetVelocity(Vector2(horizontalInput * 100.0f, velocity.y));
}

void KenRollState::Exit(FSMContext& context)
{
	Vector2 velocity = context.rigidBody2d->GetVelocity();

	context.rigidBody2d->SetVelocity(Vector2(0.0f, velocity.y));
}
