#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/CircleCollider.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/PlayerInputSystem.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/ComponentSystem.h"
#include "../D2DEngineLib/SoundManager.h"

#include "Player.h"
#include "PlayerJumpState.h"


PlayerJumpState::PlayerJumpState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerJumpState::Enter(FSMContext& context)
{
	m_jumpUpTimer = 0.0f;

	m_isStateChanged = false;

	m_stateTransitEnd = false;

	context.intParams[L"PlayerDirection"] = 1;

	context.animator->Play(m_player->m_animPath[1]);
	
	context.rigidBody2d->SetVelocity({ 0,0 });

	ComponentSystem::Get().PlayerInput().SetIsStopped(true);

	if (context.previousStateName == L"Dash" || context.previousStateName == L"Evade")
	{
		SoundManager::Get().StopSound(L"PlayerDashing");
		SoundManager::Get().PlaySound(L"PlayerDashEnd", SoundType::SFX);
	}
}

void PlayerJumpState::Update(FSMContext& context)
{
	if (m_isStateChanged) return;
	float dTime = MyTime::DeltaTime();
	
	m_walkTimer += dTime;

	if (m_walkTimer <= m_walkDuration)
	{
		context.rigidBody2d->SetVelocity(m_player->m_JumpStartDirection*1000);
		m_player->GetGameObject()->GetComponent<CircleCollider>()->SetTrigger(true);
	}

	m_jumpUpTimer += dTime;
	
	if (m_jumpUpTimer >= m_jumpUpDuration )
	{
		context.boolParams[L"JumpingUp"] = false;

		//마지막에 state 바꿀 때만 walk로 
		m_isStateChanged = true;
		Vector2 pos = context.rigidBody2d->GetPosition();
		Vector2 oneVector = { -0.7071f, -0.3536f };
		Vector2 destPos = pos + (oneVector * 50.0f) + Vector2{-80,0};
		m_player->ChangeStateAndAnim(L"Idle");
		m_player->m_isColliding = false;

		//{ -0.7071f, -0.3536f }, 1
		//{ 0.7071f, -0.3536f }, 3

		context.rigidBody2d->SetPosition(destPos);
		ComponentSystem::Get().PlayerInput().SetIsStopped(false);
		return;
	}
}

void PlayerJumpState::Exit(FSMContext& context)
{
	context.gameObject->GetComponent<CircleCollider>()->SetTrigger(false);
	//context.rigidBody2d->SetVelocity({ 0.0f, 0.0f });
}
