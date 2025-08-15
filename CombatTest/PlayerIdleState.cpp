#include "../D2DEngineLib/framework.h"
#include "PlayerIdleState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/ComponentSystem.h"
#include "../D2DEngineLib/CircleCollider.h"

#include "Player.h"
#include "DummyEnemy.h"

PlayerIdleState::PlayerIdleState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerIdleState::Enter(FSMContext& context)
{

	m_isStateChanged = false;

	m_walkDecEnd = false;

	m_dashDecEnd = false;

	m_walkEndTimer = 0.0f;

	m_dashEndTimer = 0.0f;

	bool m_stateTransitEnd = false;

	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Idle");
	context.textRenderer->SetText(stateText);

	m_player->m_currPlayerSpeed = 0.0f;
}

void PlayerIdleState::Update(FSMContext& context)
{
	if (m_isStateChanged) return;


	if (context.previousStateName == L"Walk" && m_player->m_fullSpeedWalk == true)
	{
		AfterWalkDec(context);
		m_dashDecEnd = true;

	}
	else if (context.previousStateName == L"Dash" || context.previousStateName == L"Evade")
	{
		AfterDashDec(context);
		m_walkDecEnd = true;

	}
	else if (!m_stateTransitEnd)
	{
		m_player->SetAnimState(m_player->PlayerAnimState::PlayerAnimIdle);
		context.rigidBody2d->SetVelocity(Vector2::Zero);
		m_player->m_currPlayerSpeed = 0.0f;
		m_walkDecEnd = true;
		m_dashDecEnd = true;
		m_stateTransitEnd = true;
	}

	if (m_player->m_isComboWait)
	{
		m_player->m_comboWaitTimer += MyTime::DeltaTime();

		if (m_player->m_comboWaitTimer <= m_player->m_comboWaitDuration)
		{
			if (context.triggerParams[L"NormalAttackTwice"])
			{
				Debug::Log("아이들 스테이트에서 다음으로 진행");
				m_player->m_isNextAttackQueued = false;
				m_player->m_isComboWait = false;
				m_player->m_comboWaitTimer = 0.0f;
				m_isStateChanged = true;
				m_player->ChangeStateAndAnim(L"NormalAttackTwice");
				return;
			}
			else if (context.triggerParams[L"NormalAttackThreeTimes"])
			{
				m_player->m_isNextAttackQueued = false;
				m_player->m_isComboWait = false;
				m_player->m_comboWaitTimer = 0.0f;
				m_isStateChanged = true;
				m_player->ChangeStateAndAnim(L"NormalAttackThreeTimes");
				return;
			}
		}
		else if (m_player->m_comboWaitTimer > m_player->m_comboWaitDuration)
		{
			m_player->m_isComboWait = false;
			m_player->m_comboWaitTimer = 0.0f;			
		}
	}
	
	if (context.triggerParams[L"NormalAttack"])
	{
		if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().attackStaminaCost &&
			m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval)
		{
			m_player->m_isComboWait = false;
			m_player->m_comboWaitTimer = 0.0f;
			m_player->m_fullSpeedWalk = true;
			m_player->m_currPlayerSpeed = m_player->GetPlayerStat().moveSpeed;
			m_isStateChanged = true;
			m_player->ChangeStateAndAnim(L"NormalAttack");
			return;
		}
	}

	if (context.triggerParams[L"NormalAttackAll"])
	{
		Debug::Log("올공");
		if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().attackStaminaCost &&
			m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval)
		{
			m_player->m_fullSpeedWalk = true;
			m_player->m_currPlayerSpeed = m_player->GetPlayerStat().moveSpeed;
			m_isStateChanged = true;
			m_player->ChangeStateAndAnim(L"NormalAttackAll");
			return;
		}

	}

	if (context.triggerParams[L"HeavyAttack"])
	{
		if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().heavyAttackStaminaCost &&
			m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval &&
			m_player->m_currentStateName != L"HeavyAttack")
		{
			m_player->m_isComboWait = false;
			m_player->m_comboWaitTimer = 0.0f;
			m_isStateChanged = true;
			m_player->ChangeStateAndAnim(L"HeavyAttack");

			return;
		}
	}

	

	float horizontalInput = context.floatParams[L"HorizontalInput"];
	float verticalInput = context.floatParams[L"VerticalInput"];

	if (horizontalInput != 0.0f || verticalInput != 0.0f)
	{
		if (context.boolParams[L"Evade"])
		{
			m_player->m_isComboWait = false;
			m_player->m_comboWaitTimer = 0.0f;
			m_isStateChanged = true;
			m_player->ChangeStateAndAnim(L"Evade");
			return;
		}
				
		m_player->m_comboWaitTimer = 0.0f;
		m_player->ChangeStateAndAnim(L"Walk");

		return;
	}
}

void PlayerIdleState::Exit(FSMContext& context)
{

}

void PlayerIdleState::AfterWalkDec(FSMContext& context)
{
	if (!m_walkDecEnd)
	{
		m_walkEndTimer += MyTime::DeltaTime();

		m_walkDecSpeed = (m_walkEndTimer / m_walkEndDuration) * m_player->GetPlayerStat().moveSpeed;

		m_player->m_currPlayerSpeed >= MyMath::EPSILON ? (m_player->GetPlayerStat().moveSpeed - m_walkDecSpeed) : (0.0f);

		if (m_walkEndTimer >= m_walkEndDuration)
		{
			m_player->SetAnimState(m_player->PlayerAnimState::PlayerAnimIdle);
			context.rigidBody2d->SetVelocity(Vector2::Zero);
			m_player->m_currPlayerSpeed = 0.0f;
			m_player->m_fullSpeedWalk = false;
			m_walkDecEnd = true;
			m_stateTransitEnd = true;
		}
	}
}

void PlayerIdleState::AfterDashDec(FSMContext& context)
{

	if (!m_dashDecEnd)
	{
		m_dashEndTimer += MyTime::DeltaTime();

		m_dashDecSpeed =
			m_player->GetPlayerStat().dashSpeed -
			((m_dashEndTimer / m_dashEndDuration) * m_player->GetPlayerStat().dashSpeed);

		m_player->m_currPlayerSpeed = m_dashDecSpeed >= MyMath::EPSILON ? m_dashDecSpeed : (0.0f);

		if (m_dashEndTimer >= m_dashEndDuration)
		{
			m_player->SetAnimState(m_player->PlayerAnimState::PlayerAnimIdle);
			context.rigidBody2d->SetVelocity(Vector2::Zero);
			m_player->m_currPlayerSpeed = 0.0f;
			m_dashDecEnd = true;
			m_stateTransitEnd = true;
		}

		Debug::Log("대쉬 - 아이들 감속", m_player->m_currPlayerSpeed);
	}

}
