#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Physics.h"

#include "TempEnemyAttack.h"
#include "EnemyBase.h"
#include "DummyP.h"
#include "EnemyInteract.h"

#include "DummyPlayerAttack.h"

void DummyPlayerAttack::Start()
{
	Vector2 position = GetTransform()->GetLocalPosition();

	m_debugTextObject = CreateGameObject(L"DebugText");
	m_debugTextObject->GetTransform()->SetLocalPosition(position + m_debugTextDirection * 100.0f);

	auto textRenderer = m_debugTextObject->AddComponent<TextRenderer>();
	textRenderer->SetText(L"Searching");
	textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
	textRenderer->SetSortOrder(20);
	textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Aquamarine));

}

void DummyPlayerAttack::Update()
{
	if (m_attackState == AttackState::Searching)
	{
		m_dummyAttackTimer += MyTime::DeltaTime();
		if (m_dummyAttackTimer >= m_dummySearchingDuration)
		{
			m_dummyAttackTimer = 0.0f;

			if (m_foundEnemyAttack)
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

	if (m_attackState != AttackState::Searching)
	{
		m_dummyAttackTimer += MyTime::DeltaTime();
		if (m_dummyAttackTimer >= m_dummyAttackDuration)
		{
			Destroy(GetGameObject());
			Destroy(m_debugTextObject);

			return;
		}
	}
}

void DummyPlayerAttack::OnCollisionEnter(const Collision& collision)
{
	Debug::Log(L"DummyPlayerAttack OnCollisionEnter");
}


void DummyPlayerAttack::OnTriggerEnter(const Collision& collision)
{
	Debug::Log(L"DummyPlayerAttack OnTriggerEnter");

	if (collision.otherGameObject->GetName() == L"DummyEnemyAttack")
	{
		auto comp = collision.otherGameObject->GetComponent<TempEnemyAttack>();
		if (comp->GetAttackState() == AttackState::Searching)
		{
			m_foundEnemyAttack = true;
		}
	}

	if (collision.otherGameObject->GetName() == L"EnemyBase")
	{
		auto enemyComp = collision.otherGameObject->GetComponent<EnemyBase>();
		
		//enemyComp->EnemyIA()->HitEnemy(collision, m_player, this->GetGameObject()->GetName());
	}

}

AttackState DummyPlayerAttack::GetAttackState() const
{
	return m_attackState;
}

void DummyPlayerAttack::SetTextDirection(const Vector2& direction)
{
	m_debugTextDirection = direction;
}

