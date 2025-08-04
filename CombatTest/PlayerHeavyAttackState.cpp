#include "../D2DEngineLib/framework.h"
#include "PlayerHeavyAttackState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"
#include "PlayerHeavyAttack.h"

PlayerHeavyAttackState::PlayerHeavyAttackState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerHeavyAttackState::Enter(FSMContext& context)
{
	m_eventDone = false;

	int directionId = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(directionId) + std::wstring(L" HeavyAttack");

	context.textRenderer->SetText(stateText);

	m_player->GetPlayerStatus().currentStamina -= m_player->GetPlayerStat().heavyAttackStaminaCost;

	Vector2 direction = Player::CalculateDirectionVector(directionId);

	auto normalAttackGo = m_player->CreateGameObject(L"PlayerHeavyAttack");
	normalAttackGo->GetTransform()->SetLocalPosition(m_player->GetTransform()->GetLocalPosition());
	auto rb = normalAttackGo->AddComponent<RigidBody2D>();
	rb->SetGravityScale(0.0f);
	auto coneCollider = normalAttackGo->AddComponent<ConeCollider2D>();
	coneCollider->SetCone(200.0f * direction.Length(), direction, 10.0f);
	coneCollider->SetTrigger(true);
	coneCollider->SetLayer(CollisionLayer::PlayerAttack);
	auto comp = normalAttackGo->AddComponent<PlayerHeavyAttack>();
	comp->SetDirection(direction);
	comp->SetPlayer(m_player);

	m_isStoppedPressing = false;
}

void PlayerHeavyAttackState::Update(FSMContext& context)
{
	if (!m_isSecondAttack)
	{
		if (!m_isStoppedPressing && context.boolParams[L"ContinueHeavyAttack"])
		{
			m_secondAttackTimer += MyTime::FixedDeltaTime();
			if (m_secondAttackTimer >= m_secondAttackCheckTime)
			{
				m_doSecondAttack = true;
			}
		}
		else
		{
			m_isStoppedPressing = true;
		}
	}

	m_dummyAttackAnimationTimer += MyTime::FixedDeltaTime();
	if (m_dummyAttackAnimationTimer >= m_dummyAttackAnimationDuration)
	{
		m_dummyAttackAnimationTimer = 0.0f;

		if (m_isSecondAttack)
		{
			m_isSecondAttack = false;
		}

		if (m_doSecondAttack)
		{
			m_isSecondAttack = true;
			m_doSecondAttack = false;

			if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().heavyAttackStaminaCost &&
				m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval)
			{
				context.nextStateName = L"HeavyAttack";

				context.shouldChangeState = true;
			}

			return;
		}

		float horizontalInput = context.floatParams[L"HorizontalInput"];
		float verticalInput = context.floatParams[L"VerticalInput"];

		if (horizontalInput != 0.0f || verticalInput != 0.0f)
		{
			context.nextStateName = L"Walk";
		}
		else
		{
			context.nextStateName = L"Idle";
		}

		context.shouldChangeState = true;

		return;
	}
}

void PlayerHeavyAttackState::Exit(FSMContext& context)
{
	m_player->GetPlayerStatus().attackIntervalTimer = 0.0f;
}
