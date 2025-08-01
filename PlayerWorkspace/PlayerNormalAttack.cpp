#include "../D2DEngineLib/framework.h"
#include "PlayerNormalAttack.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Physics.h"

#include "DummyEnemyAttack.h"

void PlayerNormalAttack::Start()
{
	Vector2 position = GetTransform()->GetLocalPosition();

	m_debugTextObject = CreateGameObject(L"DebugText");
	m_debugTextObject->GetTransform()->SetLocalPosition(position + m_debugTextDirection * 100.0f);
	auto textRenderer = m_debugTextObject->AddComponent<TextRenderer>();
	textRenderer->SetText(L"Searching");
	textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
}

void PlayerNormalAttack::Update()
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

void PlayerNormalAttack::OnTriggerEnter(const Collision& collision)
{
	if (collision.otherGameObject->GetName() == L"DummyEnemyAttack")
	{
		auto comp = collision.otherGameObject->GetComponent<DummyEnemyAttack>();
		if (comp->GetAttackState() == AttackState::Searching)
		{
			m_foundEnemyAttack = true;
		}
	}
}

AttackState PlayerNormalAttack::GetAttackState() const
{
	return m_attackState;
}

void PlayerNormalAttack::SetDirection(const Vector2& direction)
{
	m_debugTextDirection = direction;
}
