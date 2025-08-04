#include "../D2DEngineLib/framework.h"
#include "PlayerNormalAttack.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Physics.h"

#include "EnemyBase.h"
#include "EnemyInteract.h"

#include "DummyEnemyAttack.h"

#include "TempEnemyAttack.h"

#include "PlayerHeavyAttack.h"

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
	

	if (collision.otherGameObject->GetName() == L"EnemyAttackTest")
	{
		auto comp = collision.otherGameObject->GetComponent<TempEnemyAttack>();		
		
		auto enemyComp = comp->GetEnemyBase();

		if (comp->GetAttackState() == AttackState::Searching)
		{			
			m_foundEnemyAttack = true;			
			enemyComp->EnemyIA()->Parried(collision, m_player, this->GetGameObject()->GetName());
		}

		return;
	}

	if (collision.otherGameObject->GetName() == L"EnemyBase")
	{			
		auto enemyComp = collision.otherGameObject->GetComponent<EnemyBase>();

		enemyComp->EnemyIA()->HitEnemy(collision, m_player, this->GetGameObject()->GetName());		
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

void PlayerNormalAttack::SetPlayer(Player* player)
{
	m_player = player;
}

void PlayerNormalAttack::SetDirection(const Vector2& direction)
{
	m_debugTextDirection = direction;
}
