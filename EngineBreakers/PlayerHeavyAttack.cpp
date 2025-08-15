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
#include "PlayerAttackExplosionEffect.h"
#include "PlayerHeavyAttack.h"

void PlayerHeavyAttack::Initialize()
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

void PlayerHeavyAttack::Start()
{
	m_atkEnd = false;

	m_isOnAMove = false;

	m_attackTimer = 0.0f;

	m_moveReadyDuration = 0.0f;

	Vector2 position = GetTransform()->GetLocalPosition();

	//m_debugTextObject = CreateGameObject(L"DebugText");
	//m_debugTextObject->GetTransform()->SetLocalPosition(position + m_debugTextDirection * 100.0f);
	//auto textRenderer = m_debugTextObject->AddComponent<TextRenderer>();
	//textRenderer->SetText(L"Safety");
	//textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
	//textRenderer->SetSpaceType(SpaceType::World);
	//textRenderer->SetSortOrder(4);

	//AssaultStartPos = m_player->GetTransform()->GetLocalPosition();
	//AssaultEndPos = AssaultStartPos + (m_player->m_attackDirection * 150.0f);
}

void PlayerHeavyAttack::Update()
{
	if (m_atkEnd) return;

	float dTime = MyTime::DeltaTime();
	m_attackTimer += dTime;

	if (m_attackTimer <= m_moveReadyDuration)
	{
		return;
	}
	else if (!m_isOnAMove && m_attackTimer < m_safetyDuration + m_moveReadyDuration)
	{
		m_isOnAMove = true;
	}

	m_rigidbodyHeavyAttack->SetPosition(m_player->m_normalAttackColliderPos);

	ColliderMoveUpdate();

	if (m_player->m_attackHit)
	{
		m_isOnAMove = false;
	}

	//AssaultMove();

	if (m_attackTimer >= m_safetyDuration + m_moveReadyDuration && m_attackState == AttackState::Safety)
	{
		m_attackState = AttackState::Searching;
		m_isEffective = true;
	}

	if (m_isEffective)
	{

		if (m_attackTimer >= m_endDuration + m_moveReadyDuration)
		{
			m_isEffective = false;
			if (m_attackState == AttackState::Searching)
				m_attackState = AttackState::ENDATTACK;
		}
	}

	if (m_attackState == AttackState::ENDATTACK || m_attackState != AttackState::Parrying || m_attackState != AttackState::Dealing)
	{
		if (m_attackTimer >= m_totalAttackDuration + m_moveReadyDuration)
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

void PlayerHeavyAttack::OnTriggerStay(const Collision& collision)
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
					comp->SetIsNoDmg();
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

					Debug::Log("효과적 몸통 공격");
					auto comp = collision.otherGameObject->GetComponent<EnemyBase>();
					comp->SetIsGetDealed();

					m_attackState = AttackState::Dealing;					
					m_enemyHitChecked = true;
					m_player->m_attackHit = true;

				}
			}
		}
	}
}

void PlayerHeavyAttack::OnTriggerEnter(const Collision& collision)
{

}

AttackState PlayerHeavyAttack::GetAttackState() const
{
	return m_attackState;
}

Player* PlayerHeavyAttack::GetPlayer() const
{
	return m_player;
}

void PlayerHeavyAttack::SetCollider(ConeCollider2D* col)
{
	m_colliderHeavyAttack = col;
}

ConeCollider2D* PlayerHeavyAttack::GetCollider()
{
	return m_colliderHeavyAttack;
}

void PlayerHeavyAttack::SetRigidBody(RigidBody2D* rb)
{
	m_rigidbodyHeavyAttack = rb;
}

RigidBody2D* PlayerHeavyAttack::GetRigidBody()
{
	return m_rigidbodyHeavyAttack;
}

void PlayerHeavyAttack::SetPlayer(Player* player)
{
	m_player = player;
}

void PlayerHeavyAttack::SetDirection(const Vector2& direction)
{
	m_debugTextDirection = direction;

	m_startPosition = m_player->m_normalAttackColliderPos;

	m_startPosition = m_startPosition - (m_player->m_attackDirection * 120.0f);

	//Vector2 tV;
	//if (direction.LengthSq() <= MyMath::EPSILON)
	//{

	//}

	Vector2 tV = direction.Normalized();

	m_endPosition = m_startPosition + tV * 120.0f;

	m_moveDuration = m_safetyDuration + 0.04f;

	m_moveTimer = 0.0f;
	m_isMoving = true;
}

void PlayerHeavyAttack::ColliderMoveUpdate()
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
	else
	{
		GetRigidBody()->SetPosition(m_endPosition);
	}
}

void PlayerHeavyAttack::AssaultMove()
{
	if (m_isOnAMove)
	{
		m_moveTimer += MyTime::DeltaTime();

		float t = m_aMoveTimer / m_aMoveDuration;

		t = min(t, 1.0f);

		m_player->m_currPlayerSpeed = 300.0f * t;

		//Vector2 newPosition = Vector2::Lerp(AssaultStartPos, AssaultEndPos, t);
		//m_player->GetGameObject()->GetComponents<RigidBody2D>()SetPosition(newPosition);

		if (m_moveTimer >= m_moveDuration)
		{
			m_isOnAMove = false;
		}

	}
}

