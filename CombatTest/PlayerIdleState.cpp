#include "../D2DEngineLib/framework.h"
#include "PlayerIdleState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"

PlayerIdleState::PlayerIdleState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerIdleState::Enter(FSMContext& context)
{
	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Idle");

	context.textRenderer->SetText(stateText);
}

void PlayerIdleState::Update(FSMContext& context)
{
	if (context.triggerParams[L"HeavyAttack"])
	{
		if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().heavyAttackStaminaCost &&
			m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval)
		{
			context.nextStateName = L"HeavyAttack";

			context.shouldChangeState = true;

			return;
		}
	}

	if (context.triggerParams[L"NormalAttack"])
	{
		if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().attackStaminaCost &&
			m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval)
		{
			context.nextStateName = L"NormalAttack";

			context.shouldChangeState = true;

			return;
		}
	}

	float horizontalInput = context.floatParams[L"HorizontalInput"];
	float verticalInput = context.floatParams[L"VerticalInput"];

	if (horizontalInput != 0.0f || verticalInput != 0.0f)
	{
		context.nextStateName = L"Walk";

		context.shouldChangeState = true;

		return;
	}

	m_player->GetPlayerStatus().currentStamina += m_player->GetPlayerStat().staminaRestoreAmountPerSecond * MyTime::FixedDeltaTime();
	if (m_player->GetPlayerStatus().currentStamina > m_player->GetPlayerStat().maxStamina)
	{
		m_player->GetPlayerStatus().currentStamina = m_player->GetPlayerStat().maxStamina;
	}
}

void PlayerIdleState::Exit(FSMContext& context)
{

}