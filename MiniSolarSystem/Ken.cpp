#include "../D2DEngineLib/framework.h"
#include "Ken.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/FSMContext.h"

#include "Earth.h"
#include "Health.h"
#include "JumpingText.h"
#include "LandingText.h"

#include "KenIdleState.h"
#include "KenBackDashState.h"
#include "KenFrontDashState.h"
#include "KenRollState.h"
#include "KenSpinningKickState.h"
#include "KenHurtState.h"
#include "KenDeadState.h"
#include "KenReviveState.h"

void Ken::Initialize()
{
	m_animator = GetGameObject()->GetComponent<Animator>();
	m_health = GetGameObject()->GetComponent<Health>();
	m_rigidBody = GetGameObject()->GetComponent<RigidBody2D>();
}

void Ken::Start()
{
	// [Tip] PlayerInput Component 사용법

	PlayerInput* playerInput = GetGameObject()->GetComponent<PlayerInput>();
	playerInput->RegisterDirectionAction(DirectionInputType::Arrow, this, &Ken::ArrowInput);
	playerInput->RegisterActionOnKey('Q', KeyState::Pressed, this, &Ken::Roll);
	playerInput->RegisterActionOnKey('W', KeyState::Pressed, this, &Ken::SpinningKick);
	playerInput->RegisterActionOnKey('E', KeyState::Pressed, this, &Ken::TakeDamage);
	playerInput->RegisterActionOnKey('R', KeyState::Pressed, this, &Ken::Revive);

	playerInput->RegisterActionOnKey('4', KeyState::Pressed, this, &Ken::ChangeScene);

	// [Tip] Animator Component 사용법

	m_animator->SetSpriteSheet(L"ken_sprites.json");
	m_animator->AddAnimationClip(L"ken_idle_anim.json");
	m_animator->AddAnimationClip(L"ken_front_dash_anim.json");
	m_animator->AddAnimationClip(L"ken_back_dash_anim.json");
	m_animator->AddAnimationClip(L"ken_roll_anim.json");
	m_animator->AddAnimationClip(L"ken_spinning_kick_anim.json");
	m_animator->AddAnimationClip(L"ken_hurt_anim.json");
	m_animator->AddAnimationClip(L"ken_dead_anim.json");
	m_animator->AddAnimationClip(L"ken_revive_anim.json");

	m_animator->AddActionOnEvent(L"FireEarth", this, &Ken::FireEarth);


	// [Tip] FSM 사용법

	m_context.gameObject = GetGameObject();
	m_context.animator = m_animator;
	m_context.transform = GetTransform();
	m_context.bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
	m_context.rigidBody2d = m_rigidBody;
	m_context.floatParams[L"HorizontalInput"] = 0.0f;
	m_context.floatParams[L"VerticalInput"] = 0.0f;
	m_context.triggerParams[L"Roll"] = false;
	m_context.triggerParams[L"SpinningKick"] = false;
	m_context.triggerParams[L"Hurt"] = false;
	m_context.boolParams[L"IsDead"] = false;
	m_context.triggerParams[L"Revive"] = false;
	m_context.boolParams[L"IsGround"] = false;

	m_fsm.AddState<KenIdleState>(L"Idle", false);
	m_fsm.AddState<KenFrontDashState>(L"FrontDash", false);
	m_fsm.AddState<KenBackDashState>(L"BackDash", false);
	m_fsm.AddState<KenRollState>(L"Roll", false);
	m_fsm.AddState<KenSpinningKickState>(L"SpinningKick", false);
	m_fsm.AddState<KenHurtState>(L"Hurt", true);
	m_fsm.AddState<KenDeadState>(L"Dead", true);
	m_fsm.AddState<KenReviveState>(L"Revive", false);

	m_fsm.SetState(L"Idle", m_context);

	m_health->SetHp(50, 50);
}

void Ken::FixedUpdate()
{
	m_context.boolParams[L"IsGround"] = m_isGround;
	m_isGround = false;

	m_fsm.Update(m_context);
}

void Ken::Update()
{

}

void Ken::OnCollisionEnter(const Collision& collision)
{
	if (collision.otherGameObject->GetName() == L"Floor")
	{
		if (m_rigidBody->GetVelocity().y < 0.001f && !m_context.boolParams[L"IsGround"] && collision.normal.y < 0.0f)
		{
			GameObject* go = CreateGameObject(L"LandingText");
			LandingText* landingText = go->AddComponent<LandingText>();
			landingText->SetDirection(Vector2(-2.0f, 1.0f).Normalized());
			go->GetTransform()->SetLocalPosition(collision.contactPoint);

			go = CreateGameObject(L"LandingText");
			landingText = go->AddComponent<LandingText>();
			landingText->SetDirection(Vector2(2.0f, 1.0f).Normalized());
			go->GetTransform()->SetLocalPosition(collision.contactPoint);
		}
	}
}

void Ken::OnCollisionStay(const Collision& collision)
{
	if (collision.otherGameObject->GetName() == L"Floor")
	{
		if (m_rigidBody->GetVelocity().y < 0.001f && collision.normal.y < 0.0f)
		{
			m_isGround = true;
		}
	}
}

void Ken::OnCollisionExit(const Collision& collision)
{
	if (collision.otherGameObject->GetName() == L"Floor")
	{
		if (collision.otherGameObject->GetName() == L"Floor")
		{
			if (m_rigidBody->GetVelocity().y > 0.001f && m_context.boolParams[L"IsGround"] && collision.normal.y < 0.0f)
			{
				GameObject* go = CreateGameObject(L"JumpingText");
				go->AddComponent<JumpingText>();
				go->GetTransform()->SetLocalPosition(collision.contactPoint);
			}
		}
	}
}

void Ken::OnTriggerEnter(const Collision& collision)
{
	Debug::Log(L"Ken OnTriggerEnter");
}

void Ken::OnTriggerExit(const Collision& collision)
{
	Debug::Log(L"Ken OnTriggerExit");
}

void Ken::ArrowInput(Vector2 input)
{
	m_context.floatParams[L"HorizontalInput"] = input.x;
	m_context.floatParams[L"VerticalInput"] = input.y;
}

void Ken::Roll()
{
	m_context.triggerParams[L"Roll"] = true;
}

void Ken::SpinningKick()
{
	m_context.triggerParams[L"SpinningKick"] = true;
}

void Ken::ChangeScene()
{
	SceneManager::Get().ChangeScene(L"KinematicTest");
}

void Ken::FireEarth()
{
	GameObject* go = CreateGameObject(L"Earth");
	go->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition() + Vector2(50.0f, 70.0f));
	Earth* earth = go->AddComponent<Earth>();
	earth->Fired();
	Destroy(go, 1.0f);
}

void Ken::TakeDamage()
{
	m_health->TakeDamage(10, false);

	if (m_health->GetHp() == 0)
	{
		m_context.boolParams[L"IsDead"] = true;
	}
	else
	{
		m_context.triggerParams[L"Hurt"] = true;
	}
}

void Ken::Revive()
{
	m_context.triggerParams[L"Revive"] = true;
	m_context.boolParams[L"IsDead"] = false;
}