#include "../D2DEngineLib/framework.h"
#include "PlayerKnockbackState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"

PlayerKnockbackState::PlayerKnockbackState(Player* player)
	: PlayerStateBase(player)
{
}

bool PlayerKnockbackState::CheckCondition(FSMContext& context)
{
	return m_player->GetPlayerStatus().currentHp > 0 && 
		context.triggerParams[L"Knockback"] && !context.boolParams[L"Invincibility"];
}

void PlayerKnockbackState::Enter(FSMContext& context)
{
	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Knockback");

	context.textRenderer->SetText(stateText);

	m_knockbackDirection = -Player::CalculateDirectionVector(direction);
	m_startPosition = context.rigidBody2d->GetPosition();
	m_endPosition = m_startPosition + m_knockbackDirection * m_tempKnockbackDistance;
}

void PlayerKnockbackState::Update(FSMContext& context)
{
	float t = m_tempKnockbackTimer / m_tempKnockbackDuration;

	float easeOutValue = std::powf(1.0f - t, 2.0f);

	constexpr float scale = 3.0f;

	Vector2 direction = (m_endPosition - m_startPosition).Normalized();
	float speed = (m_tempKnockbackDistance * scale * easeOutValue) / m_tempKnockbackDuration;

	Vector2 knockbackVelocity = direction * speed;

	m_tempKnockbackTimer += MyTime::FixedDeltaTime();

	if (m_tempKnockbackTimer >= m_tempKnockbackDuration)
	{
		m_tempKnockbackTimer = 0.0f;

		float horizontalInput = context.floatParams[L"HorizontalInput"];
		float verticalInput = context.floatParams[L"VerticalInput"];

		if (horizontalInput == 0.0f && verticalInput == 0.0f)
		{
			m_player->ChangeStateAndAnim(L"Idle");
			//context.nextStateName = L"Idle";
			//context.shouldChangeState = true;

			return;
		}

		if (horizontalInput != 0.0f || verticalInput != 0.0f)
		{
			if (context.boolParams[L"Dash"])
			{
				m_player->ChangeStateAndAnim(L"Dash");
				//context.nextStateName = L"Dash";
			}
			else
			{
				m_player->ChangeStateAndAnim(L"Walk");
				//context.nextStateName = L"Walk";
			}

			//context.shouldChangeState = true;

			return;
		}
	}
	else
	{
		context.rigidBody2d->SetVelocity(knockbackVelocity);
	}
}

void PlayerKnockbackState::Exit(FSMContext& context)
{
	context.rigidBody2d->SetVelocity({ 0.0f, 0.0f });
}
