#include "../D2DEngineLib/framework.h"
#include "PlayerEvadeState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"

PlayerEvadeState::PlayerEvadeState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerEvadeState::Enter(FSMContext& context)
{
	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Evade");

	context.textRenderer->SetText(stateText);

	m_dodgeDirection = Player::CalculateDirectionVector(direction);
	m_startPosition = context.rigidBody2d->GetPosition();
	m_endPosition = m_startPosition + m_dodgeDirection * m_player->GetPlayerStat().evadeDistance;

	m_player->GetPlayerStatus().evadeIntervalTimer = 0.0f;

	m_player->GetPlayerStatus().currentStamina -= m_player->GetPlayerStat().evadeStamina;
}

void PlayerEvadeState::Update(FSMContext& context)
{
	Vector2 dodgeVelocity = (m_endPosition - m_startPosition) * 1.0f / m_player->GetPlayerStat().evadeDuration;

	m_player->GetPlayerStatus().evadeDurationTimer += MyTime::FixedDeltaTime();

	if (m_player->GetPlayerStatus().evadeDurationTimer >= m_player->GetPlayerStat().evadeDuration)
	{
		m_player->GetPlayerStatus().evadeDurationTimer = 0.0f;

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
		context.rigidBody2d->SetVelocity(dodgeVelocity);
	}
}

void PlayerEvadeState::Exit(FSMContext& context)
{
	context.rigidBody2d->SetVelocity({ 0.0f, 0.0f });
}
