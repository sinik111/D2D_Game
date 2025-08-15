#include "../D2DEngineLib/framework.h"
#include "DummyEnemy.h"

#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "DummyEnemyAttack.h"

void DummyEnemy::Update()
{
	m_dummyAttackTimer += MyTime::DeltaTime();
	if (m_dummyAttackTimer >= m_dummyAttackInterval)
	{
		m_dummyAttackTimer = 0.0f;

		auto dummyEnemyAttack = CreateGameObject(L"DummyEnemyAttack");
		dummyEnemyAttack->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition());
		auto comp = dummyEnemyAttack->AddComponent<DummyEnemyAttack>();
		comp->SetDirection(Vector2::EllipseLeftDown);
		auto collider = dummyEnemyAttack->AddComponent<ConeCollider2D>();
		collider->SetLayer(CollisionLayer::EnemyAttack);
		collider->SetCone(200.0f * Vector2::EllipseLeftDown.Length(), Vector2::EllipseLeftDown, 50.0f);
		collider->SetTrigger(true);
		auto rb = dummyEnemyAttack->AddComponent<RigidBody2D>();
		rb->SetGravityScale(0.0f);
	}
}

void DummyEnemy::OnTriggerEnter(const Collision& collision)
{
	if (collision.otherGameObject->GetName() == L"PlayerNormalAttack")
	{
		Debug::Log(L"Enemy hit");
	}
}