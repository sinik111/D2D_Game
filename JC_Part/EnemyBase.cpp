#include "../D2DEngineLib/framework.h"
#include "EnemyBase.h"

#include <limits>
#include <iostream>

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/FSMContext.h"

#include "EnemyBaseMove.h"
#include "EnemyBaseIdle.h"

#include "EnemyOutOfArea.h"



void EnemyBase::Initialize()
{
	m_playerInput = GetGameObject()->GetComponent<PlayerInput>();
	m_animator = GetGameObject()->GetComponent<Animator>();
	m_rigidBody = GetGameObject()->GetComponent<RigidBody2D>();
	m_textRenderer = GetGameObject()->GetComponent<TextRenderer>();

	m_textRenderer->SetText(L"IdleState : 2");
	//m_textRenderer->GetTransform()->SetLocalPosition(m_rigidBody->GetPosition());
	m_textRenderer->SetFontSize(15.f);
	m_textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
	m_textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
	m_textRenderer->SetRectSize({ 120.0f, 40.0f });
	m_textRenderer->SetSpaceType(SpaceType::World);
	m_textRenderer->SetSortOrder(10);
	m_textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));

	//콜라이더
	m_collider = GetGameObject()->GetComponent<BoxCollider2D>();

}

void EnemyBase::Start()
{
	m_playerInput->RegisterDirectionAction(DirectionInputType::Arrow, this, &EnemyBase::ArrowInput);

	m_animator->SetSpriteSheet(L"EnemyBase_sprites.json");

	m_animator->AddAnimationClip(L"EnemyBase_Move_UP_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_DOWN_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_LEFT_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_RIGHT_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_RIGHT_UP_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_RIGHT_DOWN_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_LEFT_UP_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_LEFT_DOWN_anim.json");

	animPath[0] = L"EnemyBase_Move_UP";
	animPath[1] = L"EnemyBase_Move_LEFT_DOWN";
	animPath[2] = L"EnemyBase_Move_DOWN";
	animPath[3] = L"EnemyBase_Move_RIGHT_DOWN";
	animPath[4] = L"EnemyBase_Move_LEFT";
	animPath[5] = L"EnemyBase_Move_UP";
	animPath[6] = L"EnemyBase_Move_RIGHT";
	animPath[7] = L"EnemyBase_Move_LEFT_UP";
	animPath[8] = L"EnemyBase_Move_UP";
	animPath[9] = L"EnemyBase_Move_RIGHT_UP";

	m_context.gameObject = GetGameObject();
	m_context.animator = m_animator;
	m_context.transform = GetTransform();
	m_context.bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
	m_context.rigidBody2d = m_rigidBody;
	m_context.floatParams[L"HorizontalInput"] = 0.0f;
	m_context.floatParams[L"VerticalInput"] = 0.0f;

	m_context.intParams[L"NextEnemyState"] = m_nextEnemyState;
	m_context.intParams[L"CurrEnemyState"] = m_currEnemyState;
	m_context.intParams[L"PrevEnemyState"] = m_prevEnemyState;

	m_context.textRenderer = m_textRenderer;
	m_textRenderer->SetText(L"IdleState : 2");

	m_rigidBody->SetGravityScale(0.0f);

	m_context.gameObject->GetTransform()->SetLocalScale({ 0.5f, 0.5f });

	PositionInit(500.0f, 200.0f, 180.0f);
	m_maxRoamDistance = 500.0f;
	m_maxChaseDistance = 650.0f;

	m_fsm.AddState<EnemyOutOfArea>(L"OutOfArea", false, this);

	m_fsm.AddState<EnemyBaseIdle>(L"Idle", false);
	m_fsm.AddState<EnemyBaseMove>(L"Move", false);

	m_fsm.SetState(L"Idle", m_context);


	//콜라이더 세팅
	m_collider->SetSize({ 30.0f, 45.0f });
	m_collider->SetOffset({ 0.0f, 45.0f });

}
void EnemyBase::FixedUpdate()
{
	m_fsm.Update(m_context);
}

void EnemyBase::Update()
{
	CheckState();
	UpdateDirection();
}


void EnemyBase::ArrowInput(Vector2 input)
{
	m_context.floatParams[L"HorizontalInput"] = input.x;
	m_context.floatParams[L"VerticalInput"] = input.y;
}

void EnemyBase::CheckState()
{
	if (m_nextEnemyState == NONE)
		return;

	m_currEnemyState = m_nextEnemyState;

	switch (m_currEnemyState)
	{
	case OUTOFAREA:

		break;

	case INAREA:

		break;

	case ENGAGE:

		break;

	case ONATTACK:

		break;

	case RETURN:

		break;
	}

	m_nextEnemyState = NONE;

}


void EnemyBase::SetTargetPlayerAndCamera(GameObject* player, GameObject* camera)
{
	m_player = player;
	m_camera = camera;
}

void EnemyBase::PositionInit(float x, float y, float angle)
{
	m_rigidBody->SetPosition({ x, y });
	m_originPos = m_rigidBody->GetPosition();
	m_originAngle = angle;
}

void EnemyBase::MoveTo(const Vector2& destination)
{
	MovingDestPos() = destination;
	ToDoMove() = true;
}

void EnemyBase::StopMoving()
{
	ToDoMove() = false;
	m_rigidBody->SetVelocity(Vector2::Zero);
}

void EnemyBase::SetAnimDirection()
{
	CurrDir() = Direction();

	if (m_textRenderer)
		m_textRenderer->SetText(m_context.currentStateName + L" " + std::to_wstring(CurrDir()));

	if (CurrDir() != PrevDir())
	{
		PrevDir() = CurrDir();

		m_animator->Play(animPath[CurrDir()]);
	}
}

void EnemyBase::UpdateDirection()
{
	if (m_rotationAngle >= 337.5 || m_rotationAngle < 22.5)		m_direction = 8;			// Dir::UP;			// ↑
	else if (m_rotationAngle >= 22.5 && m_rotationAngle < 67.5)		m_direction = 9;		// Dir::RIGHT_UP;	// ↗
	else if (m_rotationAngle >= 67.5 && m_rotationAngle < 112.5)		m_direction = 6;	// Dir::RIGHT;		// →
	else if (m_rotationAngle >= 112.5 && m_rotationAngle < 157.5)	m_direction = 3;		// Dir::RIGHT_DOWN; // ↘
	else if (m_rotationAngle >= 157.5 && m_rotationAngle < 202.5)	m_direction = 2;		// Dir::DOWN;		// ↓
	else if (m_rotationAngle >= 202.5 && m_rotationAngle < 247.5)	m_direction = 1;		// Dir::LEFT_DOWN;	// ↙
	else if (m_rotationAngle >= 247.5 && m_rotationAngle < 292.5)	m_direction = 4;		// Dir::LEFT;		// ←
	else if (m_rotationAngle >= 292.5 && m_rotationAngle < 337.5)	m_direction = 7;		// Dir::LEFT_UP;	// ↖	

	SetAnimDirection();
}

void EnemyBase::SetRotationAngle(float angle)
{
	m_rotationAngle = angle;

	if (m_rotationAngle < 0.0f) m_rotationAngle += 360.0f;
}

void EnemyBase::SetDirection(int n)
{
	m_direction = n;

	if (m_direction == 8)		m_rotationAngle = 0.0f;
	else if (m_direction == 9)	m_rotationAngle = 45.0f;
	else if (m_direction == 6)	m_rotationAngle = 90.0f;
	else if (m_direction == 3)	m_rotationAngle = 135.0f;
	else if (m_direction == 2)	m_rotationAngle = 180.0f;
	else if (m_direction == 1)	m_rotationAngle = 225.0f;
	else if (m_direction == 4)	m_rotationAngle = 270.0f;
	else if (m_direction == 7)	m_rotationAngle = 315.0f;
}



void EnemyBase::OnCollisionEnter(const Collision& collision)
{

}

void EnemyBase::OnCollisionStay(const Collision& collision)
{

}

void EnemyBase::OnCollisionExit(const Collision& collision)
{

}

void EnemyBase::OnTriggerEnter(const Collision& collision)
{
	Debug::Log(L"EnemyBase OnTriggerEnter");
}

void EnemyBase::OnTriggerExit(const Collision& collision)
{
	Debug::Log(L"EnemyBase OnTriggerExit");
}

RigidBody2D* EnemyBase::RigidBody()
{
	return m_rigidBody;
}

const Vector2& EnemyBase::Pos()
{
	return m_rigidBody->GetTransform()->GetLocalPosition();
}