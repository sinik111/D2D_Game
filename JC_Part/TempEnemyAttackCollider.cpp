#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/FSMContext.h"

#include <limits>
#include <iostream>

#include "EnemyBase.h"
#include "EnemyState.h"

#include "TempEnemyAttackCollider.h"




void TempEnemyAttackCollider::Initialize()
{	
	m_rigidBody = GetGameObject()->GetComponent<RigidBody2D>();
	m_rigidBody->SetGravityScale(0.0f);

	float parentAngleRadians = m_parentEnemy->RotationAngle() * (3.1415926535f / 180.0f);

	Vector2 directionVector = Vector2(std::cosf(parentAngleRadians), std::sinf(parentAngleRadians));
	
	float attackPointOffset = 10.0f;
	Vector2 offsetVector = directionVector * attackPointOffset;

	m_rigidBody->SetPosition(m_parentEnemy->Pos() + offsetVector);
		

	m_textRenderer = GetGameObject()->GetComponent<TextRenderer>();
	m_textRenderer->SetText(L"!!ENEMY ATTACK!!");
	m_textRenderer->SetFontSize(20.0f);
	m_textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
	m_textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
	m_textRenderer->SetRectSize({ 120.0f, 40.0f });
	m_textRenderer->SetSpaceType(SpaceType::World);
	m_textRenderer->SetSortOrder(12);
	m_textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));

	//콜라이더
	m_collider = GetGameObject()->GetComponent<BoxCollider2D>();

}

void TempEnemyAttackCollider::Start()
{
	m_context.gameObject = GetGameObject();
	m_context.transform = GetTransform();	
	
	m_context.rigidBody2d = m_rigidBody;
	m_context.textRenderer = m_textRenderer;
	
	//콜라이더 세팅
	m_collider->SetSize({ 30.0f, 45.0f });
	m_collider->SetOffset({ 0.0f, 45.0f });

}
void TempEnemyAttackCollider::FixedUpdate()
{
	
}

void TempEnemyAttackCollider::Update()
{	
	
}


void TempEnemyAttackCollider::OnCollisionEnter(const Collision& collision)
{

}

void TempEnemyAttackCollider::OnCollisionStay(const Collision& collision)
{

}

void TempEnemyAttackCollider::OnCollisionExit(const Collision& collision)
{

}

void TempEnemyAttackCollider::OnTriggerEnter(const Collision& collision)
{
	//Debug::Log(L"TempEnemyAttackCollider OnTriggerEnter");
}

void TempEnemyAttackCollider::OnTriggerExit(const Collision& collision)
{
	//Debug::Log(L"TempEnemyAttackCollider OnTriggerExit");
}