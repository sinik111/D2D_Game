#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/BatchRenderer.h"
#include "../D2DEngineLib/Particle.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/SoundManager.h"
#include "../D2DEngineLib/Camera.h"

#include "Player.h"

#include "EnemyBase.h"
#include "EnemyBaseAttack.h"
#include "PlayerCameraController.h"
#include "PlayerNormalAttack.h"
#include "PlayerAttackExplosionEffect.h"

void PlayerNormalAttack::Initialize()
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


void PlayerNormalAttack::Start()
{
	m_atkEnd = false;

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
}

void PlayerNormalAttack::Update()
{
	if (m_atkEnd) return;

	float dTime = MyTime::DeltaTime();
	m_attackTimer += dTime;

	m_rigidbodyNormalAttack->SetPosition(m_player->m_normalAttackColliderPos);

	ColliderMoveUpdate();

	if (m_attackTimer >= m_safetyDuration && m_attackState == AttackState::Safety)
	{		
		m_attackState = AttackState::Searching;
		//m_debugTextObject->GetComponent<TextRenderer>()->SetText(L"Searching");
		m_isEffective = true;		
	}

	if (m_isEffective)
	{
		if (m_attackTimer >= m_endDuration)
		{
			m_isEffective = false;
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

	if (m_parringChecked)
	{
		m_parringChecked = false;

		Vector2 position = GetTransform()->GetLocalPosition();

		auto effectGo = CreateGameObject(L"ParryingEffect");
		effectGo->GetTransform()->SetLocalPosition(position + m_debugTextDirection * 100.0f);

		auto batchRenderer = effectGo->AddComponent<BatchRenderer>();
		batchRenderer->SetLocalRect({ -100.0f, -100.0f, 100.0f, 100.0f });
		batchRenderer->SetSortOrder(4);

		auto particle = effectGo->AddComponent<Particle>();
		particle->SetBitmap(L"TestEffect/ParryingEffectTest.png");
		particle->SetSpriteSheet(L"TestEffect/ParryingEffectTest_sprites.json");
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
}

void PlayerNormalAttack::OnTriggerStay(const Collision& collision)
{	

	if (m_isEffective)
	{
		if (collision.otherGameObject->GetName() == L"EnemyBaseAttack")
		{
			auto comp = collision.otherGameObject->GetComponent<EnemyBaseAttack>();

			if (!comp->GetChecked())
			{				
				comp->SetChecked();

				if (comp->GetIsJustTime())
				{					
					comp->SetIsNoDmgHeavy();
					m_parringChecked = true;
					m_player->m_attackHit = true;
					m_attackState = AttackState::Parrying;					
				}
			}
		}

		if (collision.otherGameObject->GetName() == L"EnemyBase"
			|| collision.otherGameObject->GetName() == L"EliteEnemy"
			|| collision.otherGameObject->GetName() == L"ZakoEnemy"
			|| collision.otherGameObject->GetName() == L"BossEnemy")
		{
			if (!m_enemyHitChecked) {
				{
					GameObject* explosion = m_player->CreateGameObject(L"Explosion");
					explosion->GetTransform()->SetLocalPosition(collision.contactPoint);
					explosion->AddComponent<PlayerAttackExplosionEffect>();

					Camera::s_mainCamera->GetGameObject()->GetComponent<PlayerCameraController>()->ShakeCamera(CameraShakeType::Random, 10.0f, 0.2f, 30, true);
					SoundManager::Get().PlaySound(L"PlayerAttackSuccess", SoundType::SFX);
					auto comp = collision.otherGameObject->GetComponent<EnemyBase>();
					comp->SetIsGetDealedHeavy();

					m_attackState = AttackState::Dealing;					
					m_enemyHitChecked = true;
					m_player->m_attackHit = true;

				}
			}
		}
	}
}

void PlayerNormalAttack::OnTriggerEnter(const Collision& collision)
{

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

	m_startPosition = m_player->m_normalAttackColliderPos;

	m_startPosition = m_startPosition - (m_player->m_attackDirection * 100.0f);

	Vector2 tV = direction.Normalized();

	m_endPosition = m_startPosition + tV * 120.0f;

	m_moveDuration = m_safetyDuration + 0.03f;

	m_moveTimer = 0.0f;
	m_isMoving = true;
}

void PlayerNormalAttack::ColliderMoveUpdate()
{
	if (m_isMoving)
	{
		m_moveTimer += MyTime::DeltaTime();

		float t = m_moveTimer / m_moveDuration;

		t = min(t, 1.0f);

		Vector2 newPosition = Vector2::Lerp(m_startPosition, m_endPosition, t);
		GetRigidBody()->SetPosition(newPosition);

		if (m_moveTimer >= m_moveDuration)
		{
			m_isMoving = false;
		}
	}
}
