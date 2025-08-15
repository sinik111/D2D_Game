#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/PlayerInputSystem.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/ComponentSystem.h"
#include "../D2DEngineLib/SoundManager.h"
#include "PlayerJumpDownState.h"
#include "Player.h"

PlayerJumpDownState::PlayerJumpDownState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerJumpDownState::Enter(FSMContext& context)
{
	m_jumpDownTimer = 0.0f;
	
	m_isStateChanged = false;

	m_stateTransitEnd = false;
	
	context.intParams[L"PlayerDirection"] = 3;

	int direction = context.intParams[L"PlayerDirection"];

	context.animator->Play(m_player->m_animPath[3]);

	context.rigidBody2d->SetVelocity({ 0,0 });

	ComponentSystem::Get().PlayerInput().SetIsStopped(true);

	if (context.previousStateName == L"Dash" || context.previousStateName == L"Evade")
	{
		SoundManager::Get().StopSound(L"PlayerDashing");
		SoundManager::Get().PlaySound(L"PlayerDashEnd", SoundType::SFX);
	}
}

void PlayerJumpDownState::Update(FSMContext& context)
{
	if (m_isStateChanged) return;

	float dTime = MyTime::DeltaTime();

	m_jumpDownTimer += dTime;
	
	if (m_jumpDownTimer >= m_jumpDownDurtion)
	{
		context.boolParams[L"JumpingDown"] = false;

		m_isStateChanged = true;
		Vector2 pos = context.rigidBody2d->GetPosition();
		Vector2 oneVector = { 0.7071f, -0.3536f };
		Vector2 destPos = pos + (oneVector * 200.0f);
		m_player->ChangeStateAndAnim(L"Idle");
		m_player->m_isColliding = false;
		
		//{ -0.7071f, -0.3536f }, 1
		//{ 0.7071f, -0.3536f }, 3

		context.rigidBody2d->SetPosition(destPos);
		ComponentSystem::Get().PlayerInput().SetIsStopped(false);
		return;
	}
}

void PlayerJumpDownState::Exit(FSMContext& context)
{
}
