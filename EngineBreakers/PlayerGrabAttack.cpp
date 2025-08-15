#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/BatchRenderer.h"
#include "../D2DEngineLib/Particle.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"

#include "EnemyBase.h"

#include "PlayerGrabAttack.h"


void PlayerGrabAttack::Start()
{
	m_atkEnd = false;

	m_grabCheckTimer = 0.0f;
	m_foundKnockdownEnemy = false;

	Vector2 position = GetTransform()->GetLocalPosition();

	m_debugTextObject = CreateGameObject(L"DebugText");
	m_debugTextObject->GetTransform()->SetLocalPosition(position + m_debugTextDirection * -200.0f);
	auto textRenderer = m_debugTextObject->AddComponent<TextRenderer>();
	textRenderer->SetText(L"Knockdown Check");
	textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
	textRenderer->SetSpaceType(SpaceType::World);
	textRenderer->SetSortOrder(4);
}

void PlayerGrabAttack::Update()
{
	if (m_atkEnd) return;

	if (m_player->m_currentStateName == L"Evade")
	{
		Destroy(GetGameObject());
		Destroy(m_debugTextObject);
		m_atkEnd = true;

		return;
	}

	GetRigidBody()->SetPosition(m_player->m_normalAttackColliderPos);

	m_colliderNormalAttack->GetTransform()->SetLocalPosition(m_player->m_normalAttackColliderPos);

	
	m_grabCheckTimer += MyTime::DeltaTime();

	if (m_grabCheckTimer >= m_grabCheckDuration)
	{
		Destroy(GetGameObject());
		Destroy(m_debugTextObject);
		m_atkEnd = true;

		return;
		//if (m_foundKnockdownEnemy)
		//{
		//	return;
		//}
	}
	
}

void PlayerGrabAttack::OnTriggerEnter(const Collision& collision)
{
	//if (collision.otherGameObject->GetName() == L"EnemyBaseAttack")
	//{
	//	auto comp = collision.otherGameObject->GetComponent<EnemyBaseAttack>();
	//	m_enemyBase = comp->GetEnemyBase();

	//	if (comp->GetAttackState() == AttackState::Searching)
	//	{
	//		m_foundEnemyAttack = true;
	//		return;
	//	}
	//}

	//if (collision.otherGameObject->GetName() == L"EnemyBase")
	//{
	//	m_enemyBase = collision.otherGameObject->GetComponent<EnemyBase>();
	//	m_enemyHitChecked = true;
	//	return;
	//}
}


Player* PlayerGrabAttack::GetPlayer() const
{
	return m_player;
}

void PlayerGrabAttack::SetCollider(ConeCollider2D* col)
{
	m_colliderNormalAttack = col;
}

ConeCollider2D* PlayerGrabAttack::GetCollider()
{
	return m_colliderNormalAttack;
}

void PlayerGrabAttack::SetRigidBody(RigidBody2D* rb)
{
	m_rigidbodyNormalAttack = rb;
}

RigidBody2D* PlayerGrabAttack::GetRigidBody()
{
	return m_rigidbodyNormalAttack;
}

void PlayerGrabAttack::SetPlayer(Player* player)
{
	m_player = player;
}

void PlayerGrabAttack::SetDirection(const Vector2& direction)
{
	m_debugTextDirection = direction;
}
