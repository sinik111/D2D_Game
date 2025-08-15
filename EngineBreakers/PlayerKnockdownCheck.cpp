#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/BatchRenderer.h"
#include "../D2DEngineLib/Particle.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"

#include "EnemyBase.h"

#include "PlayerKnockdownCheck.h"


void PlayerKnockdownCheck::Initialize()
{
	GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition());

	SetRigidBody(GetGameObject()->AddComponent<RigidBody2D>());
	GetRigidBody()->SetGravityScale(0.0f);

	SetCollider(GetGameObject()->AddComponent<ConeCollider2D>());
	GetCollider()->SetCone(m_coneRadius * m_player->m_attackDirection.Length(), m_player->m_attackDirection, m_coneAngle);
	GetCollider()->SetTrigger(true);
	GetCollider()->SetLayer(CollisionLayer::PlayerAttack);

	SetDirection(m_player->m_attackDirection);
}

void PlayerKnockdownCheck::Start()
{	
	m_atkEnd = false;

	m_grabCheckTimer = 0.0f;
	m_knockdownEnemies.clear(); // ���� �� ���� �ʱ�ȭ

	Vector2 position = GetTransform()->GetLocalPosition();	
}

void PlayerKnockdownCheck::Update()
{
	if (m_atkEnd) return;

	GetRigidBody()->SetPosition(m_player->m_normalAttackColliderPos);
	m_colliderCheckKnockdown->GetTransform()->SetLocalPosition(m_player->m_normalAttackColliderPos);

	m_grabCheckTimer += MyTime::DeltaTime();


	if (m_grabCheckTimer >= m_grabCheckDuration)
	{
		FindClosestTarget();

		Destroy(GetGameObject());		
		m_atkEnd = true;
		return;
	}
}

void PlayerKnockdownCheck::OnTriggerEnter(const Collision& collision)
{
	if (collision.otherGameObject->GetName() == L"EnemyBase"
		|| collision.otherGameObject->GetName() == L"EnemyElite"
		|| collision.otherGameObject->GetName() == L"EnemyZako"
		|| collision.otherGameObject->GetName() == L"EnemyBoss")
	{
		EnemyBase* enemy = collision.otherGameObject->GetComponent<EnemyBase>();

		if (enemy != nullptr && enemy->IsKnockdown() && enemy->IsDead() == false)
		{
			m_knockdownEnemies.push_back(enemy);
		}
	}
}

void PlayerKnockdownCheck::FindClosestTarget()
{
	Debug::Log("�˴ٿ� üũ ������ ã��");
	if (m_knockdownEnemies.empty())
	{
		Debug::Log("�˴ٿ� üũ ����");
		m_player->m_isReadyGrabAttack = false;
		m_player->m_grabAttackTarget = nullptr;
		return;
	}

	float maxDotValue = -1.0f;
	EnemyBase* bestTarget = nullptr;
	Vector2 playerDirection = m_player->m_attackDirection; // �÷��̾��� ���� ����
	playerDirection.Normalize();

	for (EnemyBase* enemy : m_knockdownEnemies)
	{
		Vector2 enemyToPlayer = enemy->GetTransform()->GetLocalPosition() - m_player->GetTransform()->GetLocalPosition();
		enemyToPlayer.Normalize();

		float dotValue = Vector2::Dot(playerDirection, enemyToPlayer);

		if (dotValue > maxDotValue) // minAngle -> dotValue�� Ŭ���� ������ �� ����� ��
		{
			maxDotValue = dotValue;
			if (IsValid(enemy) && bestTarget->IsDead() == false)
			{
				bestTarget = enemy;
			}
		}
	}

	// ������ Ÿ���� ã�Ҵٸ� �÷��̾��� Ÿ������ ����
	if (bestTarget != nullptr && bestTarget->IsDead() == false)
	{
		Debug::Log("�˴ٿ� üũ ������ �ѳ� �ɸ�");
		m_player->m_isReadyGrabAttack = true;
		m_player->m_grabAttackTarget = bestTarget;
	}
}


Player* PlayerKnockdownCheck::GetPlayer() const
{
	return m_player;
}

void PlayerKnockdownCheck::SetCollider(ConeCollider2D* col)
{
	m_colliderCheckKnockdown = col;
}

ConeCollider2D* PlayerKnockdownCheck::GetCollider()
{
	return m_colliderCheckKnockdown;
}

void PlayerKnockdownCheck::SetRigidBody(RigidBody2D* rb)
{
	m_rigidbodyCheckKnockdown = rb;
}

RigidBody2D* PlayerKnockdownCheck::GetRigidBody()
{
	return m_rigidbodyCheckKnockdown;
}

void PlayerKnockdownCheck::SetPlayer(Player* player)
{
	m_player = player;
}

void PlayerKnockdownCheck::SetDirection(const Vector2& direction)
{
	m_debugTextDirection = direction;
}
