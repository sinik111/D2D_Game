#include "../D2DEngineLib/framework.h"
#include "PlayerHeavyAttack.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Physics.h"

#include "DummyEnemyAttack.h"
#include "TempEnemyAttack.h"

void PlayerHeavyAttack::Start()
{
	Vector2 position = GetTransform()->GetLocalPosition();

	m_debugTextObject = CreateGameObject(L"DebugText");
	m_debugTextObject->GetTransform()->SetLocalPosition(position + m_debugTextDirection * 100.0f);
	auto textRenderer = m_debugTextObject->AddComponent<TextRenderer>();
	textRenderer->SetText(L"Searching");
	textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
}

void PlayerHeavyAttack::Update()
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

void PlayerHeavyAttack::OnTriggerEnter(const Collision& collision)
{
	if (collision.otherGameObject->GetName() == L"TempEnemyAttack")
	{
		auto comp = collision.otherGameObject->GetComponent<TempEnemyAttack>();
		if (comp->GetAttackState() == AttackState::Searching)
		{
			m_foundEnemyAttack = true;
		}
	}
}

AttackState PlayerHeavyAttack::GetAttackState() const
{
	return m_attackState;
}

Player* PlayerHeavyAttack::GetPlayer() const
{
	return m_player;
}

void PlayerHeavyAttack::SetPlayer(Player* player)
{
	m_player = player;
}

void PlayerHeavyAttack::SetDirection(const Vector2& direction)
{
	m_debugTextDirection = direction;
}
