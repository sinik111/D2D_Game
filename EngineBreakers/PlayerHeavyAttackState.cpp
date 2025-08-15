#include "../D2DEngineLib/framework.h"
#include "PlayerHeavyAttackState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"
#include "PlayerHeavyAttack.h"

PlayerHeavyAttackState::PlayerHeavyAttackState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerHeavyAttackState::Enter(FSMContext& context)
{	
	m_eventDone = false;

	m_isStateChanged = false;

	m_player->m_onAttack = true;

	m_attackAnimationTimer = 0.0f;	

	m_secondAttackTimer = 0.0f;

	m_attackAnimationDuration = m_player->m_attackHeavyDuration;

	m_player->m_isOnHeavyAttackOnce = true;

	m_player->m_lastAttackName = L"HeavyAttack";

	m_player->PlayerHeavyAttack1(m_attackAnimationDuration);
}

void PlayerHeavyAttackState::Update(FSMContext& context)
{
	if (m_isStateChanged) return;

	float dTime = MyTime::DeltaTime();

	m_attackAnimationTimer += dTime;
	
	if (m_player->m_isReadyHeavyAttackTwice)
	{
		m_secondAttackTimer += dTime;
	}

	if (!m_player->m_isReadyHeavyAttackTwice)
	{
		m_secondAttackTimer = 0.0f;
	}

	//CheckCancel(context);
	//if (m_isStateChanged) return;

	if ((context.rigidBody2d->GetPosition() - m_player->m_attackStartPos).LengthSq() >= m_player->m_attackMoveDistSq
		|| m_player->m_attackHit)
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
		
		if (m_secondAttackTimer > 0.95f)
		{			
			m_secondAttackTimer = 0.0f;
			m_player->ChangeStateAndAnim(L"HeavyAttackTwice"); 
		}
		else
		{
			m_player->GetPlayerStatus().attackIntervalTimer = 0.0f;
			m_player->ChangeStateWalkOrIdle();
			return;
		}
	}
}

void PlayerHeavyAttackState::Exit(FSMContext& context)
{
	m_player->m_onAttack = false;
	m_player->m_isOnHeavyAttackOnce = false;
	m_player->m_isReadyHeavyAttackTwice = false;
	
}

void PlayerHeavyAttackState::CheckCancel(FSMContext& context)
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
