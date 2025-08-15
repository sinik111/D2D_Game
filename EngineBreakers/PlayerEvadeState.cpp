#include "../D2DEngineLib/framework.h"
#include "PlayerEvadeState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/SoundManager.h"

#include "Player.h"
#include "PlayerDashDustEffect.h"

PlayerEvadeState::PlayerEvadeState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerEvadeState::Enter(FSMContext& context)
{
	m_isStateChanged = false;

	m_player->GetPlayerStatus().evadeDurationTimer = 0.0f;

	m_dashTransitTimer = 0.0f;

	m_evadeStartTimer = 0.0f;
	m_evadeSpeed = 0.0f;
	m_evadeAccEnd = false;

	m_dustTimer = 0.0f;
	m_stateTransitEnd = false;

	m_player->GetPlayerStatus().evadeIntervalTimer = 0.0f;
	m_player->GetPlayerStatus().currentStamina -= m_player->GetPlayerStat().evadeStamina;

	SoundManager::Get().PlaySound(L"PlayerDashIn", SoundType::SFX);
}

void PlayerEvadeState::Update(FSMContext& context)
{
	if (m_isStateChanged) return;

	m_dustTimer += MyTime::DeltaTime();
	if (m_dustTimer >= m_dustInterval)
	{
		m_dustTimer = 0.0f;

		GameObject* dust = m_player->CreateGameObject(L"Dust");
		dust->GetTransform()->SetLocalPosition(m_player->GetTransform()->GetLocalPosition());
		dust->AddComponent<PlayerDashDustEffect>();
	}

	float elapsedTime = MyTime::DeltaTime();

	m_player->GetPlayerStatus().evadeDurationTimer += elapsedTime;

	if (!m_evadeAccEnd)
	{
		m_evadeStartTimer += elapsedTime;
		m_evadeSpeed = m_player->GetPlayerStat().moveSpeed +
			((m_evadeStartTimer / m_evadeInDuration)
				* (m_player->m_evadeOnlyVelocity - m_player->GetPlayerStat().moveSpeed));


		m_player->m_currPlayerSpeed = m_evadeSpeed;

		if (m_evadeStartTimer >= m_evadeInDuration)
		{
			m_player->SetAnimState(m_player->PlayerAnimState::PlayerAnimDashIng);
			m_player->m_currPlayerSpeed = m_player->m_evadeOnlyVelocity;
			m_evadeAccEnd = true;
			m_stateTransitEnd = true;
		}
	}

	CheckCancel(context);
	if (m_isStateChanged) return;

	if (context.boolParams[L"Dash"])
	{
		m_dashTransitTimer += elapsedTime;

		if (m_dashTransitTimer >= m_dashTransitDuration)
		{
			{
				m_isStateChanged = true;
				context.triggerParams[L"Evade"] = false;
				m_player->GetPlayerStatus().evadeDurationTimer = 0.0f;
				m_player->ChangeStateAndAnim(L"Dash");
				return;
			}
		}
	}

	if (m_player->GetPlayerStatus().evadeDurationTimer >= m_player->GetPlayerStat().evadeDuration)
	{
		context.triggerParams[L"Evade"] = false;
		m_player->GetPlayerStatus().evadeDurationTimer = 0.0f;

		m_isStateChanged = true;
		m_player->ChangeStateAndAnim(L"Walk");
		return;
	}
}

void PlayerEvadeState::Exit(FSMContext& context)
{
	//context.rigidBody2d->SetVelocity({ 0.0f, 0.0f });

	if (context.nextStateName != L"Dash")
	{
		SoundManager::Get().PlaySound(L"PlayerDashEnd", SoundType::SFX);
	}
}

void PlayerEvadeState::CheckCancel(FSMContext& context)
{
	if (context.triggerParams[L"NormalAttackOnce"])
	{
		if (//m_stateTransitEnd &&
			(m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().attackStaminaCost &&
				m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval))
		{
			context.triggerParams[L"Evade"] = false;
			m_player->GetPlayerStatus().evadeDurationTimer = 0.0f;
			m_player->m_currPlayerSpeed = m_player->GetPlayerStat().moveSpeed;
			m_isStateChanged = true;
			m_player->ChangeStateAndAnim(L"NormalAttackOnce");

			return;
		}
	}

}
