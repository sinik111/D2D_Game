#include "../D2DEngineLib/framework.h"
#include "DummyP.h"
#include "DummyIdle.h"
#include "DummyPlayerAttack.h"

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

#include "../D2DEngineLib/ConeCollider2D.h"




void DummyP::Initialize()
{	
	m_playerInput = GetGameObject()->GetComponent<PlayerInput>();
	
	//m_animator = GetGameObject()->GetComponent<Animator>();
	m_bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();

	m_rigidBody = GetGameObject()->GetComponent<RigidBody2D>();
	m_text = GetGameObject()->GetComponent<TextRenderer>();
	
	m_text->SetFontSize(15.f);
	m_text->SetHorizontalAlignment(HorizontalAlignment::Center);
	m_text->SetVerticalAlignment(VerticalAlignment::Center);
	m_text->SetRectSize({ 120.0f, 40.0f });
	m_text->SetSpaceType(SpaceType::World);
	m_text->SetSortOrder(10);
	m_text->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));


	//콜라이더
	//m_collider = GetGameObject()->GetComponent<BoxCollider2D>();

	m_moveSpeed = 500.0f;

}

void DummyP::Start()
{
	m_playerInput->RegisterDirectionAction(DirectionInputType::Arrow, this, &DummyP::ArrowInput);

	m_playerInput->RegisterActionOnKey('U', KeyState::Pressed, this, &DummyP::Attack);
	
	m_context.gameObject = GetGameObject();	
	
	m_context.bitmapRenderer = m_bitmapRenderer;

	m_context.transform = GetTransform();
	m_context.bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
	m_context.rigidBody2d = m_rigidBody;
	m_context.textRenderer = m_text;
	
	m_context.floatParams[L"HorizontalInput"] = 0.0f;
	m_context.floatParams[L"VerticalInput"] = 0.0f;

	m_context.intParams[L"Direction"] = 2;	
	m_rigidBody->SetGravityScale(0.0f);

	m_text->SetText(L"[Dummy Player]");

	m_rigidBody->SetPosition({ 0.0f, 0.0f });	

	m_fsm.AddState<DummyIdle>(L"Idle", false);	
	m_fsm.SetState(L"Idle", m_context);


	//콜라이더 세팅
	//m_collider->SetSize({ 30.0f, 45.0f });
	//m_collider->SetOffset({ 0.0f, 45.0f });

}
void DummyP::FixedUpdate()
{
	m_fsm.Update(m_context);
}

void DummyP::Update()
{
	MoveByArrowInput();	
}

void DummyP::Attack()
{
	auto dpAttack = CreateGameObject(L"DummyPlayerAttack");

	Vector2 playerPos = GetGameObject()->GetTransform()->GetLocalPosition();	

	auto attackColliderPosition = playerPos + (m_inputDirection * 60.0f);

	dpAttack->GetTransform()->SetLocalPosition(attackColliderPosition);

	auto comp = dpAttack->AddComponent<DummyPlayerAttack>(this);
	comp->SetTextDirection(Vector2::EllipseLeftDown);

	auto collider = dpAttack->AddComponent<ConeCollider2D>();
	collider->SetLayer(CollisionLayer::EnemyAttack);
	collider->SetCone(200.0f * Vector2::EllipseLeftDown.Length(), m_inputDirection, 100.0f);
	collider->SetTrigger(true);
	collider->SetLayer(CollisionLayer::PlayerAttack);

	auto rb = dpAttack->AddComponent<RigidBody2D>();
	rb->SetGravityScale(0.0f);
}


void DummyP::ArrowInput(Vector2 input)
{
	m_context.floatParams[L"HorizontalInput"] = input.x;
	m_context.floatParams[L"VerticalInput"] = input.y;
}


void DummyP::MoveByArrowInput()
{
	if (!m_rigidBody || !m_context.floatParams.count(L"HorizontalInput")) return;

	float horizontalInput = m_context.floatParams[L"HorizontalInput"];
	float verticalInput = m_context.floatParams[L"VerticalInput"];

	Vector2 dir(horizontalInput, verticalInput);

	
	if (dir == Vector2::Zero) 
	{
		m_rigidBody->SetVelocity(Vector2::Zero); 
		return;
	}

	dir.Normalize();
			
	Vector2 velocity = dir * m_moveSpeed;
		
	m_rigidBody->SetVelocity(velocity);

	m_inputDirection = dir;
}



void DummyP::SetDirectionByRotation(float angle)
{
	// atan2는 y 먼저, x 나중	
	m_rotationAngle = angle;	

	if (m_rotationAngle < 0.0f) m_rotationAngle += 360.0f;

	float& degree = m_rotationAngle;

	// 시계방향으로 8방향 판정. 0도가 UP, 180도가 DOWN
	if (degree >= 337.5 || degree < 22.5)  m_direction = Dir::UP; // ↑
	if (degree >= 22.5 && degree < 67.5) m_direction = Dir::RIGHT_UP; // ↗
	if (degree >= 67.5 && degree < 112.5)  m_direction = Dir::RIGHT; // →
	if (degree >= 112.5 && degree < 157.5)   m_direction = Dir::RIGHT_DOWN; // ↘
	if (degree >= 157.5 && degree < 202.5)  m_direction = Dir::DOWN; // ↓
	if (degree >= 202.5 && degree < 247.5)  m_direction = Dir::LEFT_DOWN; // ↙
	if (degree >= 247.5 && degree < 292.5) m_direction = Dir::LEFT; // ←
	if (degree >= 292.5 && degree < 337.5) m_direction = Dir::LEFT_UP; // ↖

	m_context.intParams[L"Direction"] = m_direction;
}


void DummyP::OnCollisionEnter(const Collision& collision)
{

}

void DummyP::OnCollisionStay(const Collision& collision)
{

}

void DummyP::OnCollisionExit(const Collision& collision)
{

}

void DummyP::OnTriggerEnter(const Collision& collision)
{
	Debug::Log(L"DummyP OnTriggerEnter");
}

void DummyP::OnTriggerExit(const Collision& collision)
{
	Debug::Log(L"DummyP OnTriggerExit");
}