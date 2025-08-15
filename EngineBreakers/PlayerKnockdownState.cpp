#include "../D2DEngineLib/framework.h"
#include "PlayerKnockdownState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"

PlayerKnockdownState::PlayerKnockdownState(Player* player)
	: PlayerStateBase(player)
{
}

bool PlayerKnockdownState::CheckCondition(FSMContext& context)
{
	return  m_player->GetPlayerStatus().currentHp > 0 && 
		context.triggerParams[L"Knockdown"] && !context.boolParams[L"Invincibility"];
}

void PlayerKnockdownState::Enter(FSMContext& context)
{
	int direction = context.intParams[L"PlayerDirection"];

	m_isStateChanged = false;

	m_knockdownTimer = 0.0f;
	

	//m_knockbackDirection = -Player::CalculateDirectionVector(direction);
	//m_startPosition = context.rigidBody2d->GetPosition();
	//m_endPosition = m_startPosition + m_knockbackDirection * m_tempKnockdownDistance;

	//m_knockbackDone = false;
}

void PlayerKnockdownState::Update(FSMContext& context)
{
	if (m_isStateChanged) return;


	//float t = m_tempKnockdownTimer / m_tempKnockdownDuration;

	//float easeOutValue = std::powf(1.0f - t, 2.0f);

	//constexpr float scale = 3.0f;

	//Vector2 direction = (m_endPosition - m_startPosition).Normalized();
	//float speed = (m_tempKnockdownDistance * scale * easeOutValue) / m_tempKnockdownDuration;

	//Vector2 knockbackVelocity = direction * speed;

	m_knockdownTimer += MyTime::DeltaTime();

	if (m_knockdownTimer >= m_knockdownDuration)
	{		
		m_isStateChanged = true;
		float horizontalInput = context.floatParams[L"HorizontalInput"];
		float verticalInput = context.floatParams[L"VerticalInput"];

		if (horizontalInput == 0.0f && verticalInput == 0.0f)
		{
			m_player->ChangeStateAndAnim(L"Idle");
			return;
		}

		if (horizontalInput != 0.0f || verticalInput != 0.0f)
		{
			if (context.boolParams[L"Evade"])
			{
				m_player->ChangeStateAndAnim(L"Evade");				
			}
			else
			{
				m_player->ChangeStateAndAnim(L"Walk");				
			}

			//context.shouldChangeState = true;

			return;
		}
	}
}

void PlayerKnockdownState::Exit(FSMContext& context)
{
	//context.rigidBody2d->SetVelocity({ 0.0f, 0.0f });
}
