#include "../D2DEngineLib/framework.h"
#include "PlayerNormalAttackState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"

#include "Player.h"
#include "PlayerNormalAttack.h"

PlayerNormalAttackState::PlayerNormalAttackState(Player* player)
	: PlayerStateBase(player)
{

}

void PlayerNormalAttackState::Enter(FSMContext& context)
{
	m_eventDone = false;

	int directionId = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(directionId) + std::wstring(L" NormalAttack");

	context.textRenderer->SetText(stateText);

	m_player->GetPlayerStatus().currentStamina -= m_player->GetPlayerStat().attackStaminaCost;

	Vector2 direction = Player::CalculateDirectionVector(directionId);

	auto normalAttackGo = m_player->CreateGameObject(L"PlayerNormalAttack");
	normalAttackGo->GetTransform()->SetLocalPosition(m_player->GetTransform()->GetLocalPosition());
	auto rb = normalAttackGo->AddComponent<RigidBody2D>();
	rb->SetGravityScale(0.0f);
	auto coneCollider = normalAttackGo->AddComponent<ConeCollider2D>();
	coneCollider->SetCone(200.0f * direction.Length(), direction, 10.0f);
	coneCollider->SetTrigger(true);
	coneCollider->SetLayer(CollisionLayer::PlayerAttack);
	auto comp = normalAttackGo->AddComponent<PlayerNormalAttack>();
	comp->SetDirection(direction);
}

void PlayerNormalAttackState::Update(FSMContext& context)
{
	m_dummyAttackAnimationTimer += MyTime::FixedDeltaTime();
	if (m_dummyAttackAnimationTimer >= m_dummyAttackAnimationDuration)
	{
		m_dummyAttackAnimationTimer = 0.0f;

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

void PlayerNormalAttackState::Exit(FSMContext& context)
{
	m_player->GetPlayerStatus().attackIntervalTimer = 0.0f;
}
