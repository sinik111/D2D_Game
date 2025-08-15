#include "../D2DEngineLib/framework.h"


#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"

#include "PlayerNormalAttackThreeTimesState.h"



PlayerNormalAttackThreeTimesState::PlayerNormalAttackThreeTimesState(Player* player)
	: PlayerStateBase(player)
{

}

void PlayerNormalAttackThreeTimesState::Enter(FSMContext& context)
{
	m_eventDone = false;

	m_isStateChanged = false;

	m_player->m_onAttack = true;

	m_player->m_attackHit = false;

	m_attackAnimationTimer = 0.0f;

	m_moveDistSq = m_player->m_attackMoveDistSq;

	m_attackAnimationDuration = m_player->m_attackNormalThreeTimesDuration;

	m_player->m_lastAttackName = L"NormalAttackThreeTimes";

	m_player->PlayerNormalAttackThreeTimes(m_attackAnimationDuration);
}

void PlayerNormalAttackThreeTimesState::Update(FSMContext& context)
{
	if (m_isStateChanged) return;

	m_attackAnimationTimer += MyTime::DeltaTime();

	CheckCancel(context);
	if (m_isStateChanged) return;



	if ((context.rigidBody2d->GetPosition() - m_player->m_attackStartPos).LengthSq() >= m_moveDistSq
		|| m_player->m_attackHit || m_player->m_isKnockback)
	{
		context.rigidBody2d->SetVelocity(Vector2::Zero);
		m_player->m_currPlayerSpeed = 0.0f;
		m_moveDistSq = 0.0f;
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


	if (m_attackAnimationTimer >= m_attackAnimationDuration)
	{
		m_isStateChanged = true;
		m_player->ChangeStateWalkOrIdle();
		return;
	}
}

void PlayerNormalAttackThreeTimesState::Exit(FSMContext& context)
{
	m_player->m_attackHit = false;
	m_player->m_isComboInputAvailable = false;
	m_player->m_isNextAttackQueued = false;
	m_player->m_isComboWait = false;
	m_player->m_comboWaitTimer = 0.0f;
	m_player->m_onAttack = false;
	m_player->GetPlayerStatus().attackIntervalTimer = 0.0f;
}

void PlayerNormalAttackThreeTimesState::CheckCancel(FSMContext& context)
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