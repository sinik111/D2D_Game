#include "../D2DEngineLib/framework.h"
#include "PlayerDashState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"

PlayerDashState::PlayerDashState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerDashState::Enter(FSMContext& context)
{
	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Dash");

	context.textRenderer->SetText(stateText);
}

void PlayerDashState::Update(FSMContext& context)
{
	float horizontalInput = context.floatParams[L"HorizontalInput"];
	float verticalInput = context.floatParams[L"VerticalInput"];

	if (horizontalInput == 0.0f && verticalInput == 0.0f)
	{
		context.nextStateName = L"Idle";

		context.shouldChangeState = true;

		return;
	}

	if (!context.boolParams[L"Dash"])
	{	
		context.nextStateName = L"Walk";

		context.shouldChangeState = true;

		return;
	}

	m_player->GetPlayerStatus().currentStamina -= m_player->GetPlayerStat().dashStaminaPerSec * MyTime::FixedDeltaTime();
	if (m_player->GetPlayerStatus().currentStamina < 0.0f)
	{
		m_player->GetPlayerStatus().currentStamina = 0.0f;

		context.nextStateName = L"Walk";

		context.shouldChangeState = true;

		return;
	}

	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Dash");

	context.textRenderer->SetText(stateText);

	float dashSpeed = m_player->GetPlayerStat().dashSpeed;

	Vector2 directionVector = Player::CalculateDirectionVector(direction);

	context.rigidBody2d->SetVelocity(directionVector * dashSpeed);
}

void PlayerDashState::Exit(FSMContext& context)
{
	context.rigidBody2d->SetVelocity({ 0.0f, 0.0f });
}
