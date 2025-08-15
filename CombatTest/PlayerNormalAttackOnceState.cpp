#include "../D2DEngineLib/framework.h"
#include "PlayerNormalAttackState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"

#include "PlayerNormalAttackOnceState.h"


PlayerNormalAttackOnceState::PlayerNormalAttackOnceState(Player* player)
	: PlayerStateBase(player)
{

}

void PlayerNormalAttackOnceState::Enter(FSMContext& context)
{
	m_eventDone = false;

	m_isStateChanged = false;

	m_player->m_onAttack = true;

	m_attackAnimationTimer = 0.0f;

	m_player->m_isComboWait = false;

	m_attackAnimationDuration = m_player->m_attackNormalOnceDuration;

	m_player->m_lastAttackName = L"NormalAttackOnce";

	m_player->PlayerNormalAttackOnce(m_attackAnimationDuration);
}

void PlayerNormalAttackOnceState::Update(FSMContext& context)
{
	if (m_isStateChanged) return;

	m_attackAnimationTimer += MyTime::DeltaTime();

	CheckCancel(context);
	if (m_isStateChanged) return;

	if ((context.rigidBody2d->GetPosition() - m_player->m_attackStartPos).LengthSq() >= m_player->m_attackMoveDistSq)
	{
		context.rigidBody2d->SetVelocity(Vector2::Zero);
		m_player->m_currPlayerSpeed = 0.0f;
	}
	else
	{
		const float speedDecay = 1.0f;

		int tvt = static_cast<int>((m_attackAnimationTimer / m_attackAnimationDuration) * 100.0f);

		tvt = tvt <= 0 ? 1 : tvt;

		m_player->m_currPlayerSpeed -= tvt * tvt * speedDecay * MyTime::DeltaTime();
		
		if (m_player->m_currPlayerSpeed <= 0.01f)
		{
			m_player->m_currPlayerSpeed = 0.0f;
		}
	}		

	const float comboInputStartTime = m_attackAnimationDuration / 3.0f;

	if (m_attackAnimationTimer >= comboInputStartTime && m_attackAnimationTimer < m_attackAnimationDuration)
	{
		m_player->m_isComboInputAvailable = true;
	}
	else
	{
		m_player->m_isComboInputAvailable = false;
	}

	if (m_attackAnimationTimer >= m_attackAnimationDuration)
	{
		m_isStateChanged = true;
		if (m_player->m_isNextAttackQueued)
		{
			Debug::Log("공격 원스 스테이트에서 다음으로 진행");
			m_player->m_isComboWait = false;
			m_player->m_isNextAttackQueued = false;			
			m_player->ChangeStateAndAnim(L"NormalAttackTwice");
		}
		else
		{
			m_player->ChangeStateWalkOrIdle();
		}
		return;
	}

}

void PlayerNormalAttackOnceState::Exit(FSMContext& context)
{
	m_player->m_isComboInputAvailable = false;
	m_player->m_isNextAttackQueued = false;	
	m_player->m_isComboWait = true;
	m_player->m_comboWaitTimer = 0.0f;
	m_player->m_onAttack = false;
	m_player->GetPlayerStatus().attackIntervalTimer = 0.0f;
}

void PlayerNormalAttackOnceState::CheckCancel(FSMContext& context)
{
	if (context.triggerParams[L"Evade"])
	{

		if (m_player->GetPlayerStatus().evadeIntervalTimer >= m_player->GetPlayerStat().evadeInterval &&
			m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().evadeStamina)
		{				
			m_player->m_currPlayerSpeed = 0.0f;
			m_isStateChanged = true;
			m_player->ChangeStateAndAnim(L"Evade");
			return;
		}
	}

}