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

#include "PlayerNormalAttack.h"
#include "PlayerHeavyAttack.h"

#include "EnemyBaseAttack.h"



void EnemyBaseAttack::Start()
{
	//Vector2 position = GetTransform()->GetLocalPosition();

	m_debugTextObject = CreateGameObject(L"DebugText");
	m_debugTextObject->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

	auto textRenderer = m_debugTextObject->AddComponent<TextRenderer>();

	textRenderer->SetText(L"Searching");

	textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
	textRenderer->SetSpaceType(SpaceType::World);
	textRenderer->SetSortOrder(4);
	textRenderer->SetSortOrder(20);
	textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Aquamarine));


}


void EnemyBaseAttack::Update()
{
	//공격 모션은 나왔으나, 아직 공격 판정이 없는 구간
	if (m_attackState == AttackState::Safety)
	{
		m_dummyAttackTimer += MyTime::DeltaTime();

		if (m_dummyAttackTimer >= m_dummySafetyDuration)
		{
			m_dummyAttackTimer = 0.0f;
			m_attackState = AttackState::Searching;
		}
		return;
	}

	//실제 공격, 또는 패링 가능한 구간
	if (m_attackState == AttackState::Searching)
	{
		m_dummyAttackTimer += MyTime::DeltaTime();
		if (m_dummyAttackTimer >= m_dummySearchingDuration)
		{
			m_dummyAttackTimer = 0.0f;

			if (m_foundPlayerAttack)
			{
				m_attackState = AttackState::Parrying;

				m_debugTextObject->GetComponent<TextRenderer>()->SetText(L"Parrying");
			}
			else
			{
				m_attackState = AttackState::Dealing;

				m_debugTextObject->GetComponent<TextRenderer>()->SetText(L"Dealing");
			}
		}
	}

	//공격모션이 마무리되고 이펙트 잔상이 남아있거나 한 상태. 공격판정은 없는 구간
	if (m_attackState != AttackState::Searching)
	{
		m_dummyAttackTimer += MyTime::DeltaTime();
		if (m_dummyAttackTimer >= m_dummyAttackDuration)
		{
			Destroy(GetGameObject());
			Destroy(m_debugTextObject);
			m_parentEnemy = nullptr;

			return;
		}
	}
}



void EnemyBaseAttack::OnTriggerEnter(const Collision& collision)
{
	if (collision.otherGameObject->GetName() == L"PlayerNormalAttack")
	{
		auto comp = collision.otherGameObject->GetComponent<PlayerNormalAttack>();
		if (comp->GetAttackState() == AttackState::Searching)
		{
			m_foundPlayerAttack = true;
		}
	}

	if (collision.otherGameObject->GetName() == L"PlayerHeaveAttack")
	{
		auto comp = collision.otherGameObject->GetComponent<PlayerHeavyAttack>();
		if (comp->GetAttackState() == AttackState::Searching)
		{
			m_foundPlayerAttack = true;
		}
	}
}

void EnemyBaseAttack::OnTriggerStay(const Collision& collision)
{
	if (collision.otherGameObject->GetName() == L"Player")
	{
		if (m_attackState == AttackState::Dealing)
		{
			//m_attackState = AttackState::Safety;
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
}