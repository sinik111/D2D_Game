#include "../D2DEngineLib/framework.h"
#include "PlayerWalkState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/ComponentSystem.h"
#include "../D2DEngineLib/CircleCollider.h"

#include "Player.h"
#include "EnemyBase.h"
//#include "DummyEnemy.h"

PlayerWalkState::PlayerWalkState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerWalkState::Enter(FSMContext& context)
{
	m_isStateChanged = false;

	m_walkAccEnd = false;

	m_dashDecEnd = false;

	m_player->m_fullSpeedWalk = false;

	m_walkStartTimer = 0.0f;

	m_dashEndTimer = 0.0f;

	m_stateTransitEnd = false;
}

void PlayerWalkState::Update(FSMContext& context)
{
	if (m_isStateChanged) return;


	if (context.previousStateName == L"Idle")
	{
		StartWalkAcc(context);
		m_dashDecEnd = true;
	}
	else if (context.previousStateName == L"Dash" || context.previousStateName == L"Evade")
	{
		AfterDashDec(context);
		m_walkAccEnd = true;
	}
	else if (!m_stateTransitEnd)
	{
		m_player->SetAnimState(m_player->PlayerAnimState::PlayerAnimWalkIng);
		m_player->m_fullSpeedWalk = true;
		m_player->m_currPlayerSpeed = m_player->GetPlayerStat().moveSpeed;
		m_dashDecEnd = true;
		m_walkAccEnd = true;
		m_stateTransitEnd = true;
	}

	if (m_player->m_isComboWait)
	{
		m_player->m_comboWaitTimer += MyTime::DeltaTime();

		if (m_player->m_comboWaitTimer <= m_player->m_comboWaitDuration)
		{
			if (context.triggerParams[L"NormalAttackTwice"])
			{
				Debug::Log("워크스테이트에서 다음으로 진행");
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
			m_player->m_isNextAttackQueued = false;
		}
	}

	if (context.triggerParams[L"NormalAttackOnce"])
	{
		if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().attackStaminaCost &&
			m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval)
		{
			m_player->m_fullSpeedWalk = true;
			m_player->m_currPlayerSpeed = m_player->GetPlayerStat().moveSpeed;
			m_isStateChanged = true;
			m_player->ChangeStateAndAnim(L"NormalAttackOnce");
			return;
		}

	}
		

	if (context.triggerParams[L"HeavyAttack"])
	{
		if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().heavyAttackStaminaCost &&
			m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval&&
			m_player->m_currentStateName != L"HeavyAttack")
		{
			m_player->m_isComboWait = false;
			m_player->m_comboWaitTimer = 0.0f;
			m_player->m_fullSpeedWalk = true;
			m_player->m_currPlayerSpeed = m_player->GetPlayerStat().moveSpeed;
			m_isStateChanged = true;
			m_player->ChangeStateAndAnim(L"HeavyAttack");
			return;
		}
	}

	if (context.triggerParams[L"Evade"])
	{
		if (m_player->GetPlayerStatus().evadeIntervalTimer >= m_player->GetPlayerStat().evadeInterval &&
			m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().evadeStamina)
		{
			m_player->m_isComboWait = false;
			m_player->m_comboWaitTimer = 0.0f;
			m_isStateChanged = true;
			m_player->ChangeStateAndAnim(L"Evade");
			return;
		}
	}

	//if (!context.boolParams[L"Dash"])
	//{
	//	if (m_canDodge &&
	//		m_player->GetPlayerStatus().evadeIntervalTimer >= m_player->GetPlayerStat().evadeInterval &&
	//		m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().evadeStamina)
	//	{
	//		m_isStateChanged = true;
	//		m_player->ChangeStateAndAnim(L"Evade");
	//		return;
	//	}
	//}

	//if (m_canDodge)
	//{
	//	m_player->GetPlayerStatus().evadeAvailalbeTimer += MyTime::FixedDeltaTime();

	//	if (m_player->GetPlayerStatus().evadeAvailalbeTimer >= m_player->GetPlayerStat().evadeAvailableTime)
	//	{
	//		m_canDodge = false;
	//		m_player->GetPlayerStatus().evadeAvailalbeTimer = 0.0f;
	//	}
	//}

	if (context.boolParams[L"JumpingUp"])
	{
		
		m_player->m_isJump = true;
		m_isStateChanged = true;
		m_player->ChangeStateAndAnim(L"JumpUp");
		return;
	}

	if (context.boolParams[L"JumpingDown"])
	{
		m_player->m_isJump = true;
		m_isStateChanged = true;
		m_player->ChangeStateAndAnim(L"JumpDown");
		return;
	}

	if (m_player->m_movingAheadDirection == 5)
	{
		if (m_dashDecEnd && m_walkAccEnd)
		{			
			m_isStateChanged = true;
			m_player->ChangeStateAndAnim(L"Idle");
			return;
		}
	}
}

void PlayerWalkState::Exit(FSMContext& context)
{
	if (!m_player->m_onHoverMode)
	{
		//m_player->m_currPlayerSpeed = 0.0f;
		//context.rigidBody2d->SetVelocity(Vector2::Zero);
	}

	m_player->GetPlayerStatus().evadeAvailalbeTimer = 0.0f;
}

void PlayerWalkState::AfterDashDec(FSMContext& context)
{

	if (!m_dashDecEnd) {

		m_dashEndTimer += MyTime::DeltaTime();

		m_dashDecSpeed = m_player->GetPlayerStat().dashSpeed - ((m_dashEndTimer / m_dashEndDuration) *
			(m_player->GetPlayerStat().dashSpeed - m_player->GetPlayerStat().moveSpeed));

		m_player->m_currPlayerSpeed =
			m_dashDecSpeed >= m_player->GetPlayerStat().moveSpeed ?
			m_dashDecSpeed : m_player->GetPlayerStat().moveSpeed;

		if (m_dashEndTimer >= m_dashEndDuration)
		{
			m_player->SetAnimState(m_player->PlayerAnimState::PlayerAnimWalkIng);
			m_dashDecEnd = true;
			m_stateTransitEnd = true;
			m_player->m_fullSpeedWalk = true;
			m_player->m_currPlayerSpeed = m_player->GetPlayerStat().moveSpeed;
		}
		
	}

}

void PlayerWalkState::StartWalkAcc(FSMContext& context)
{
	if (!m_walkAccEnd) {

		m_walkStartTimer += MyTime::DeltaTime();

		m_walkSpeed = (m_walkStartTimer / m_walkInDuration) * m_player->GetPlayerStat().moveSpeed;

		m_player->m_currPlayerSpeed = m_walkSpeed;

		if (m_walkStartTimer >= m_walkInDuration)
		{
			m_player->SetAnimState(m_player->PlayerAnimState::PlayerAnimWalkIng);
			m_player->m_currPlayerSpeed = m_player->GetPlayerStat().moveSpeed;
			m_player->m_fullSpeedWalk = true;
			m_walkAccEnd = true;
			m_stateTransitEnd = true;
		}
	}
}
