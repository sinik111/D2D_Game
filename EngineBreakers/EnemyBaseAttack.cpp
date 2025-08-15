#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/FSMContext.h"
#include "../D2DEngineLib/SoundManager.h"

#include <limits>
#include <iostream>

#include "EnemyBase.h"
#include "EnemyState.h"

#include "Player.h"

#include "EnemyBaseAttack.h"

void EnemyBaseAttack::Initialize()
{
	GetTransform()->SetLocalPosition(m_parentEnemy->m_attackColliderPosition);

	SetRigidBody(GetGameObject()->AddComponent<RigidBody2D>());
	GetRigidBody()->SetGravityScale(0.0f);

	m_enemyAttackDirection = m_parentEnemy->m_aheadDirection;

	SetCollider(GetGameObject()->AddComponent<ConeCollider2D>());
	GetCollider()->SetCone(m_coneRadius * m_enemyAttackDirection.Length(), m_enemyAttackDirection, m_coneAngle);
	GetCollider()->SetTrigger(true);
	GetCollider()->SetLayer(CollisionLayer::EnemyAttack);
	
	SetDirection(m_enemyAttackDirection);

	m_isNoDmg = false;
	m_isJustTime = false;
	m_isColliderChecked = false;
	m_atkEnd = false;
	m_playerHitChecked = false;
}



void EnemyBaseAttack::Start()
{	
	m_attackTimer = 0.0f;
	m_hitCheckTimer = 0.0f;

	Vector2 position = GetTransform()->GetLocalPosition();

	//m_debugTextObject = CreateGameObject(L"DebugText");
	//m_debugTextObject->GetTransform()->SetLocalPosition(position + m_debugTextDirection * 100.0f);
	//auto textRenderer = m_debugTextObject->AddComponent<TextRenderer>();
	//textRenderer->SetText(L"Safety");
	//textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
	//textRenderer->SetSpaceType(SpaceType::World);
	//textRenderer->SetSortOrder(4);

	m_attackState = AttackState::Safety;
}


void EnemyBaseAttack::Update()
{
	if (m_atkEnd) return;

	float dTime = MyTime::DeltaTime();
	m_attackTimer += dTime;

	
	GetRigidBody()->SetPosition(m_parentEnemy->m_attackColliderPosition);

	ColliderMoveUpdate();

	if(m_attackTimer >= m_safetyDuration && m_attackState == AttackState::Safety)
	{		
		m_attackState = AttackState::Searching;
		//m_debugTextObject->GetComponent<TextRenderer>()->SetText(L"Searching");
		m_isJustTime = true;
	}

	if (m_isNoDmg || m_playerHitChecked)
	{
		m_atkEnd = true;

		Destroy(GetGameObject());
		//Destroy(m_debugTextObject);
		return;
	}

	if (m_isJustTime)
	{
		
		if (m_attackTimer >= m_endDuration)
		{
			m_isJustTime = false;
			if (m_attackState == AttackState::Searching)
				m_attackState = AttackState::ENDATTACK;
		}
	}

	if (m_attackState == AttackState::ENDATTACK || m_attackState != AttackState::Parrying || m_attackState != AttackState::Dealing)
	{
		if (m_attackTimer >= m_totalAttackDuration)
		{
			m_atkEnd = true;

			Destroy(GetGameObject());
			//Destroy(m_debugTextObject);
			return;
		}
	}	
}



void EnemyBaseAttack::OnTriggerEnter(const Collision& collision)
{
	
}

void EnemyBaseAttack::OnTriggerStay(const Collision& collision)
{
	if (m_isJustTime && !m_isNoDmg && !m_playerHitChecked)
	{
		if (collision.otherGameObject->GetName() == L"Player")
		{
			//Debug::Log("플레이어를 때림");
			auto comp = collision.otherGameObject->GetComponent<Player>();

			if (comp->GetPlayerIsDead() == false)
			{
				if (m_isJustTime && !m_isNoDmg )
				{
					SoundManager::Get().PlaySound(L"PlayerDamaged", SoundType::SFX);

					comp->AddDmg(m_dmg);
					comp->AddKnock(m_knock, m_enemyAttackDirection);
					m_playerHitChecked = true;
				}
			}
		}
	}
}			


AttackState EnemyBaseAttack::GetAttackState() const
{
	return m_attackState;
}

EnemyBase* EnemyBaseAttack::GetEnemyBase() const
{
	return m_parentEnemy;
}

void EnemyBaseAttack::SetAttackState(AttackState state)
{
	m_attackState = state;
}


void EnemyBaseAttack::SetDirection(const Vector2& direction)
{
	m_debugTextDirection = direction;

	m_startPosition = GetTransform()->GetLocalPosition();

	Vector2 tV = direction.Normalized();
	
	m_endPosition = m_startPosition + tV * 90.0f;
	
	m_moveDuration = m_safetyDuration;
	
	m_moveTimer = 0.0f;
	m_isMoving = true;
}

void EnemyBaseAttack::ColliderMoveUpdate()
{
	if (m_isMoving)
	{
		m_moveTimer += MyTime::DeltaTime();
			
		float t = m_moveTimer / m_moveDuration;
				
		t = min(t, 1.0f);
				
		Vector2 newPosition = Vector2::Lerp(m_startPosition, m_endPosition, t);
		GetTransform()->SetLocalPosition(newPosition);
				
		if (m_moveTimer >= m_moveDuration)
		{
			m_isMoving = false;		
		}
	}
}


void EnemyBaseAttack::SetCollider(ConeCollider2D* col)
{
	m_colliderEnemyAttack = col;
}

ConeCollider2D* EnemyBaseAttack::GetCollider()
{
	return m_colliderEnemyAttack;
}

void EnemyBaseAttack::SetRigidBody(RigidBody2D* rb)
{
	m_rigidbodyEnemyAttack = rb;
}

RigidBody2D* EnemyBaseAttack::GetRigidBody()
{
	return m_rigidbodyEnemyAttack;
}