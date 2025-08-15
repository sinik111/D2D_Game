#include "../D2DEngineLib/framework.h"
#include "PlayerNormalAttack.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/BatchRenderer.h"
#include "../D2DEngineLib/Particle.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"

#include "EnemyBase.h"
#include "EnemyBaseAttack.h"
#include "PlayerHeavyAttack.h"


void PlayerNormalAttack::Start()
{
	m_atkEnd = false;
	
	Vector2 position = GetTransform()->GetLocalPosition();

	m_debugTextObject = CreateGameObject(L"DebugText");
	m_debugTextObject->GetTransform()->SetLocalPosition(position + m_debugTextDirection * 100.0f);
	auto textRenderer = m_debugTextObject->AddComponent<TextRenderer>();
	textRenderer->SetText(L"Searching");
	textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
	textRenderer->SetSpaceType(SpaceType::World);
	textRenderer->SetSortOrder(4);
}

void PlayerNormalAttack::Update()
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

	if (m_attackState == AttackState::Searching)
	{
		m_attackTimer += MyTime::DeltaTime();
		
		if (m_attackTimer >= m_searchingDuration)
		{
			m_attackTimer = 0.0f;

			if (m_foundEnemyAttack)
			{
				m_attackState = AttackState::Parrying;

				m_enemyBase->EnemyIA()->Parried(this->GetPlayer(), this->GetGameObject()->GetName());

				m_debugTextObject->GetComponent<TextRenderer>()->SetText(L"Parrying");

				Vector2 position = GetTransform()->GetLocalPosition();

				auto effectGo = CreateGameObject(L"ParryingEffect");
				effectGo->GetTransform()->SetLocalPosition(position + m_debugTextDirection * 100.0f);
				
				auto batchRenderer = effectGo->AddComponent<BatchRenderer>();
				batchRenderer->SetLocalRect({ -100.0f, -100.0f, 100.0f, 100.0f });
				batchRenderer->SetSortOrder(4);
				
				auto particle = effectGo->AddComponent<Particle>();
				particle->SetBitmap(L"ParryingEffectTest.png");
				particle->SetSpriteSheet(L"ParryingEffectTest_sprites.json");
				particle->SetDuration(0.2f);

				for (int i = 0; i < 6; ++i)
				{
					ParticleUnit particleUnit;

					particleUnit.batchUnits.push_back(
						{
							0,
							{ 0.0f, 0.0f },
							{ 0.3f, 0.3f },
							-i * 60.0f,
							{ 1.0f, 1.0f, 1.0f, 1.0f }
						}
					);

					particleUnit.batchUnits.push_back(
						{
							0,
							{ Vector2::RotateVector(Vector2::Right, i * 60.0f) * 50.0f },
							{ 0.5f, 0.5f },
							-i * 60.0f,
							{ 1.0f, 1.0f, 1.0f, 1.0f }
						}
					);

					particleUnit.duration = 0.2f;
					particleUnit.startTime = 0.0f;

					particle->AddParticleUnit(particleUnit);
				}
			}
			else
			{
				m_attackState = AttackState::Dealing;
				
				if (m_enemyHitChecked)
				{
					m_enemyBase->EnemyIA()->HitEnemy(this->GetPlayer(), this->GetGameObject()->GetName());
				}

				m_debugTextObject->GetComponent<TextRenderer>()->SetText(L"Dealing");
			}
		}
	}

	if (m_attackState != AttackState::Searching)
	{
		m_attackTimer += MyTime::DeltaTime();
		if (m_attackTimer >= m_totalAttackDuration - m_attackDuration)
		{
			Destroy(GetGameObject());
			Destroy(m_debugTextObject);
			m_atkEnd = true;

			return;
		}
	}
}

void PlayerNormalAttack::OnTriggerEnter(const Collision& collision)
{
	if (collision.otherGameObject->GetName() == L"EnemyBaseAttack")
	{
		auto comp = collision.otherGameObject->GetComponent<EnemyBaseAttack>();
		m_enemyBase = comp->GetEnemyBase();

		if (comp->GetAttackState() == AttackState::Searching)
		{			
			m_foundEnemyAttack = true;
			return;
		}		
	}

	if (collision.otherGameObject->GetName() == L"EnemyBase")
	{
		m_enemyBase = collision.otherGameObject->GetComponent<EnemyBase>();
		m_enemyHitChecked = true;
		return;
	}	
}

AttackState PlayerNormalAttack::GetAttackState() const
{
	return m_attackState;
}

Player* PlayerNormalAttack::GetPlayer() const
{
	return m_player;
}

void PlayerNormalAttack::SetCollider(ConeCollider2D* col)
{
	m_colliderNormalAttack = col;
}

ConeCollider2D* PlayerNormalAttack::GetCollider()
{
	return m_colliderNormalAttack;
}

void PlayerNormalAttack::SetRigidBody(RigidBody2D* rb)
{
	m_rigidbodyNormalAttack = rb;
}

RigidBody2D* PlayerNormalAttack::GetRigidBody()
{
	return m_rigidbodyNormalAttack;
}

void PlayerNormalAttack::SetPlayer(Player* player)
{
	m_player = player;
}

void PlayerNormalAttack::SetDirection(const Vector2& direction)
{
	m_debugTextDirection = direction;
}
