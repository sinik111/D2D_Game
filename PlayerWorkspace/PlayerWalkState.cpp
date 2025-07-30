#include "../D2DEngineLib/framework.h"
#include "PlayerWalkState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"

void PlayerWalkState::Enter(FSMContext& context)
{
	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Walk");

	context.textRenderer->SetText(stateText);
}

void PlayerWalkState::Update(FSMContext& context)
{
	float horizontalInput = context.floatParams[L"HorizontalInput"];
	float verticalInput = context.floatParams[L"VerticalInput"];

	if (horizontalInput == 0.0f && verticalInput == 0.0f)
	{
		context.nextStateName = L"Idle";

		context.shouldChangeState = true;

		return;
	}

	if (context.triggerParams[L"Dodge"])
	{
		m_canDodge = true;
	}

	if (!context.boolParams[L"Dash"])
	{
		if (m_canDodge)
		{
			context.nextStateName = L"Dodge";

			context.shouldChangeState = true;

			return;
		}
	}

	if (m_canDodge)
	{
		m_dodgeAvailableTimer += MyTime::FixedDeltaTime();

		if (m_dodgeAvailableTimer >= context.floatParams[L"DodgeAvailableTime"])
		{
			m_canDodge = false;

			m_dodgeAvailableTimer = 0.0f;
		}
	}

	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Walk");

	context.textRenderer->SetText(stateText);

	float moveSpeed = context.floatParams[L"MoveSpeed"];

	Vector2 directionVector = Player::CalculateDirectionVector(direction);

	context.rigidBody2d->SetVelocity(directionVector * moveSpeed);
}

void PlayerWalkState::Exit(FSMContext& context)
{
	context.rigidBody2d->SetVelocity({ 0.0f, 0.0f });

	m_canDodge = false;

	m_dodgeAvailableTimer = 0.0f;
}
