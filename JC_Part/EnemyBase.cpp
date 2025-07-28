#include "../D2DEngineLib/framework.h"
#include "EnemyBase.h"

#include <limits>

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/FSMContext.h"

#include "EnemyBaseMove.h"
#include "EnemyBaseIdle.h"




void EnemyBase::Initialize()
{
	m_animator = GetGameObject()->GetComponent<Animator>();	
	m_rigidBody = GetGameObject()->GetComponent<RigidBody2D>();
}

void EnemyBase::Start()
{
	PlayerInput* playerInput = GetGameObject()->GetComponent<PlayerInput>();
	playerInput->RegisterDirectionAction(DirectionInputType::Arrow, this, &EnemyBase::ArrowInput);
	//playerInput->RegisterActionOnKey('Z', KeyState::Pressed, this, &EnemyBase::Move);
	//playerInput->RegisterActionOnKey('X', KeyState::Pressed, this, &EnemyBase::SpinningKick);
	//playerInput->RegisterActionOnKey('C', KeyState::Pressed, this, &EnemyBase::TakeDamage);
	//playerInput->RegisterActionOnKey('V', KeyState::Pressed, this, &EnemyBase::Revive);
	

	m_animator->SetSpriteSheet(L"EnemyBase_sprites.json");

	m_animator->AddAnimationClip(L"EnemyBase_Move_UP_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_DOWN_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_LEFT_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_RIGHT_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_RIGHT_UP_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_RIGHT_DOWN_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_LEFT_UP_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_LEFT_DOWN_anim.json");

	//m_animator->AddAnimationClip(L"EnemyBase_Idle_UP_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Idle_DOWN_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Idle_LEFT_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Idle_RIGHT_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Idle_RIGHT_UP_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Idle_RIGHT_DOWN_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Idle_LEFT_UP_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Idle_LEFT_DOWN_anim.json");

	//m_animator->AddAnimationClip(L"EnemyBase_Attack_UP_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Attack_DOWN_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Attack_LEFT_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Attack_RIGHT_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Attack_RIGHT_UP_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Attack_RIGHT_DOWN_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Attack_LEFT_UP_anim.json");
	//m_animator->AddAnimationClip(L"EnemyBase_Attack_LEFT_DOWN_anim.json");


	m_context.gameObject = GetGameObject();
	m_context.animator = m_animator;
	m_context.transform = GetTransform();
	m_context.bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
	m_context.rigidBody2d = m_rigidBody;
	m_context.floatParams[L"HorizontalInput"] = 0.0f;
	m_context.floatParams[L"VerticalInput"] = 0.0f;
	m_context.intParams[L"Direction"] = 2;
	m_context.boolParams[L"isMove"] = false;

	m_fsm.AddState<EnemyBaseIdle>(L"IDLE");
	m_fsm.AddState<EnemyBaseMove>(L"Move");	

	m_fsm.SetState(L"IDLE", m_context);

}
void EnemyBase::FixedUpdate()
{
	m_fsm.Update(m_context);
}

void EnemyBase::Update()
{
	SetEnemyDirectionByInput();
}


void EnemyBase::ArrowInput(Vector2 input)
{
	m_context.floatParams[L"HorizontalInput"] = input.x;
	m_context.floatParams[L"VerticalInput"] = input.y;

}

void EnemyBase::SetEnemyDirectionByInput()
{
	float horizontalInput = m_context.floatParams[L"HorizontalInput"];
	float verticalInput = m_context.floatParams[L"VerticalInput"];


	// 방향 벡터가 없으면 0 (중립)
	if (horizontalInput == 0.0f && verticalInput == 0.0f)		
	{
		m_context.boolParams[L"isMove"] = false;
		return;
	}
	else
	{
		m_context.boolParams[L"isMove"] = true;
	}


	// atan2는 y 먼저, x 나중
	float angle = std::atan2(verticalInput, horizontalInput); // 라디안
	float degree = angle * 180.0f / 3.14159265f;

	if (degree < 0) degree += 360.0f;

	// 시계방향으로 8방향 판정	
	if (degree >= 247.5 && degree < 292.5)  direction = Dir::UP; // ↑
	if (degree >= 292.5 && degree < 337.5)  direction = Dir::RIGHT_UP; // ↗
	if (degree >= 337.5 || degree < 22.5)   direction = Dir::RIGHT; // →
	if (degree >= 22.5 && degree < 67.5)   direction = Dir::RIGHT_DOWN; // ↘
	if (degree >= 67.5 && degree < 112.5)  direction = Dir::DOWN; // ↓
	if (degree >= 112.5 && degree < 157.5)  direction = Dir::LEFT_DOWN; // ↙
	if (degree >= 157.5 && degree < 202.5)  direction = Dir::LEFT; // ←
	if (degree >= 202.5 && degree < 247.5)  direction = Dir::LEFT_UP; // ↖

	m_context.intParams[L"Direction"] = direction;

	return;
}


void EnemyBase::OnCollisionEnter(const Collision& collision)
{
	//if (collision.otherGameObject->GetName() == L"Floor")
	//{
	//	if (m_rigidBody->GetVelocity().y < 0.001f && !m_context.boolParams[L"IsGround"] && collision.normal.y < 0.0f)
	//	{
	//		GameObject* go = CreateGameObject(L"LandingText");
	//		LandingText* landingText = go->AddComponent<LandingText>();
	//		landingText->SetDirection(Vector2(-2.0f, 1.0f).Normalized());
	//		go->GetTransform()->SetLocalPosition(collision.contactPoint);

	//		go = CreateGameObject(L"LandingText");
	//		landingText = go->AddComponent<LandingText>();
	//		landingText->SetDirection(Vector2(2.0f, 1.0f).Normalized());
	//		go->GetTransform()->SetLocalPosition(collision.contactPoint);
	//	}
	//}
}

void EnemyBase::OnCollisionStay(const Collision& collision)
{
	//if (collision.otherGameObject->GetName() == L"Floor")
	//{
	//	if (m_rigidBody->GetVelocity().y < 0.001f && collision.normal.y < 0.0f)
	//	{
	//		m_isGround = true;
	//	}
	//}
}

void EnemyBase::OnCollisionExit(const Collision& collision)
{
	//if (collision.otherGameObject->GetName() == L"Floor")
	//{
	//	if (collision.otherGameObject->GetName() == L"Floor")
	//	{
	//		if (m_rigidBody->GetVelocity().y > 0.001f && m_context.boolParams[L"IsGround"] && collision.normal.y < 0.0f)
	//		{
	//			GameObject* go = CreateGameObject(L"JumpingText");
	//			go->AddComponent<JumpingText>();
	//			go->GetTransform()->SetLocalPosition(collision.contactPoint);
	//		}
	//	}
	//}
}

void EnemyBase::OnTriggerEnter(const Collision& collision)
{
	Debug::Log(L"EnemyBase OnTriggerEnter");
}

void EnemyBase::OnTriggerExit(const Collision& collision)
{
	Debug::Log(L"EnemyBase OnTriggerExit");
}