#include "../D2DEngineLib/framework.h"
#include "PlayerWalkState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"

PlayerWalkState::PlayerWalkState(Player* player)
	: PlayerStateBase(player)
{
}

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

	if (context.triggerParams[L"Evade"])
	{
		m_canDodge = true;
	}

	if (!context.boolParams[L"Dash"])
	{
		if (m_canDodge &&
			m_player->GetPlayerStatus().evadeIntervalTimer >= m_player->GetPlayerStat().evadeInterval &&
			m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().evadeStamina)
		{
			context.nextStateName = L"Evade";

			context.shouldChangeState = true;

			return;
		}
	}

	m_player->GetPlayerStatus().currentStamina += m_player->GetPlayerStat().staminaRestoreAmountPerSecond * MyTime::FixedDeltaTime();
	if (m_player->GetPlayerStatus().currentStamina > m_player->GetPlayerStat().maxStamina)
	{
		m_player->GetPlayerStatus().currentStamina = m_player->GetPlayerStat().maxStamina;
	}

	if (m_canDodge)
	{
		m_player->GetPlayerStatus().evadeAvailalbeTimer += MyTime::FixedDeltaTime();

		if (m_player->GetPlayerStatus().evadeAvailalbeTimer >= m_player->GetPlayerStat().evadeAvailableTime)
		{
			m_canDodge = false;

			m_player->GetPlayerStatus().evadeAvailalbeTimer = 0.0f;
		}
	}

	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Walk");

	context.textRenderer->SetText(stateText);

	float moveSpeed = m_player->GetPlayerStat().moveSpeed;

	Vector2 directionVector = Player::CalculateDirectionVector(direction);

	context.rigidBody2d->SetVelocity(directionVector * moveSpeed);
}

void PlayerWalkState::Exit(FSMContext& context)
{
	context.rigidBody2d->SetVelocity({ 0.0f, 0.0f });

	m_canDodge = false;

	m_player->GetPlayerStatus().evadeAvailalbeTimer = 0.0f;
}
