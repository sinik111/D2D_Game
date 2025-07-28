#include "../D2DEngineLib/framework.h"
#include "PlayerDodgeState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"

void PlayerDodgeState::Enter(FSMContext& context)
{
	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Dodge");

	context.textRenderer->SetText(stateText);

	m_dodgeDirection = Player::CalculateDirectionVector(direction);
	m_startPosition = context.rigidBody2d->GetPosition();
	m_endPosition = m_startPosition + m_dodgeDirection * context.floatParams[L"DodgeDistance"];
}

void PlayerDodgeState::Update(FSMContext& context)
{
	float dodgeSpeed = context.floatParams[L"DodgeSpeed"];

	Vector2 currentPosition = context.rigidBody2d->GetPosition();
	
	float remainDistanceSq = Vector2::DistanceSq(m_endPosition, currentPosition);

	Vector2 nextPosition = currentPosition +
		m_dodgeDirection * dodgeSpeed * MyTime::FixedDeltaTime();

	float nextDistanceSq = Vector2::DistanceSq(currentPosition, nextPosition);

	if (remainDistanceSq <= nextDistanceSq)
	{
		float horizontalInput = context.floatParams[L"HorizontalInput"];
		float verticalInput = context.floatParams[L"VerticalInput"];

		if (horizontalInput == 0.0f && verticalInput == 0.0f)
		{
			context.nextStateName = L"Idle";

			context.shouldChangeState = true;

			return;
		}

		if (horizontalInput != 0.0f || verticalInput != 0.0f)
		{
			if (context.boolParams[L"Dash"])
			{
				context.nextStateName = L"Dash";
			}
			else
			{
				context.nextStateName = L"Walk";
			}

			context.shouldChangeState = true;

			return;
		}
	}
	else
	{
		context.rigidBody2d->SetVelocity(m_dodgeDirection * dodgeSpeed);
	}
}

void PlayerDodgeState::Exit(FSMContext& context)
{
	context.rigidBody2d->SetVelocity({ 0.0f, 0.0f });
}
