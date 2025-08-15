#include "../D2DEngineLib/framework.h"
#include "PlayerDashState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/SoundManager.h"

#include "Player.h"
#include "PlayerDashDustEffect.h"

PlayerDashState::PlayerDashState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerDashState::Enter(FSMContext& context)
{
	m_isStateChanged = false;

	m_dashStartTimer = 0.0f;	
	
	m_dashDecEnd = false;

	m_dashRealEndTimer = 0.0f;

	m_dustTimer = 0.0f;

	m_stateTransitEnd = false;

	if (!SoundManager::Get().IsPlaying(L"PlayerDashing"))
	{
		SoundManager::Get().PlaySound(L"PlayerDashing", SoundType::SFX, nullptr, L"PlayerDashing", true);
	}
}

void PlayerDashState::Update(FSMContext& context)
{
	m_dustTimer += MyTime::DeltaTime();
	if (m_dustTimer >= m_dustInterval)
	{
		m_dustTimer = 0.0f;

		GameObject* dust = m_player->CreateGameObject(L"Dust");
		dust->GetTransform()->SetLocalPosition(m_player->GetTransform()->GetLocalPosition());
		dust->AddComponent<PlayerDashDustEffect>();
	}

	float horizontalInput = context.floatParams[L"HorizontalInput"];
	float verticalInput = context.floatParams[L"VerticalInput"];

	if (horizontalInput == 0.0f && verticalInput == 0.0f)
	{
		m_dashRealEndTimer += MyTime::DeltaTime();

		if (m_dashRealEndTimer >= m_dashRealEndDuration)
		{
			m_isStateChanged = true;
			m_player->ChangeStateAndAnim(L"Idle");
			return;
		}			
	}
	else
	{
		m_dashRealEndTimer = 0.0f;
	}	

	if (!context.boolParams[L"Dash"])
	{	
		m_isStateChanged = true;
		m_player->ChangeStateAndAnim(L"Walk");		
		return;
	}

	m_player->GetPlayerStatus().currentStamina -= 
		m_player->GetPlayerStat().dashStaminaPerSec * MyTime::DeltaTime();
	
	if (m_player->GetPlayerStatus().currentStamina < 0.0f)
	{
		m_player->GetPlayerStatus().currentStamina = 0.0f;
		m_isStateChanged = true;
		m_player->ChangeStateAndAnim(L"Walk");		
		return;
	}

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

	if (!m_dashDecEnd)
	{
		m_dashStartTimer += MyTime::DeltaTime();
		m_dashSpeed = m_player->m_evadeOnlyVelocity - ((m_dashStartTimer / m_dashInDuration)
			* (m_player->m_evadeOnlyVelocity - m_player->GetPlayerStat().dashSpeed));

		m_player->m_currPlayerSpeed = m_dashSpeed;

		if (m_dashStartTimer >= m_dashInDuration)
		{
			m_player->SetAnimState(m_player->PlayerAnimState::PlayerAnimDashIng);
			m_player->m_currPlayerSpeed = m_player->GetPlayerStat().dashSpeed;
			m_dashDecEnd = true;
			m_stateTransitEnd = true;
		}
	}	

	
}

void PlayerDashState::Exit(FSMContext& context)
{
	SoundManager::Get().PlaySound(L"PlayerDashEnd", SoundType::SFX);

	SoundManager::Get().StopSound(L"PlayerDashing");
}
