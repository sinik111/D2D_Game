#include "../D2DEngineLib/framework.h"
#include "PlayerIdleState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/ComponentSystem.h"
#include "../D2DEngineLib/CircleCollider.h"

#include "Player.h"
#include "DummyEnemy.h"

PlayerIdleState::PlayerIdleState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerIdleState::Enter(FSMContext& context)
{
	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Idle");

	context.textRenderer->SetText(stateText);
}

void PlayerIdleState::Update(FSMContext& context)
{
	if (context.triggerParams[L"HeavyAttack"] || context.triggerParams[L"NormalAttack"])
	{
		if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().attackStaminaCost &&
			m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval)
		{
			CircleCollider circleCollider;
			circleCollider.SetCircle({ context.rigidBody2d->GetPosition(), m_dummyGrabDistance});
			circleCollider.SetLayer(CollisionLayer::PlayerAttack);
			circleCollider.CalculateSpatialBounds();

			auto resultColliders = ComponentSystem::Get().Physics().DetectCollisions(&circleCollider);

			for (const auto& collider : resultColliders)
			{
				if (collider->GetGameObject()->GetName() == L"DummyEnemy")
				{
					Vector2 playerPos = context.rigidBody2d->GetPosition();
					Vector2 enemyPos = collider->GetRigidBody2D()->GetPosition();
					Vector2 toEnemy = enemyPos - playerPos;

					float distance = toEnemy.Length();
					if (m_dummyGrabDistance < distance)
					{
						continue;
					}

					Vector2 playerDir = Player::CalculateDirectionVector(context.intParams[L"PlayerDirection"]).Normalized();
					Vector2 toEnemyDir = toEnemy.Normalized();

					float dotResult = Vector2::Dot(playerDir, toEnemyDir);
					float angle = std::acosf(dotResult) * MyMath::RadToDeg;

					if (angle <= 60.0f)
					{
						// auto gameObject = collider->GetGameObject();
						// auto comp = gameObject->GetComponent<DummyEnemy>();
						// if comp-> isKnockdown check
						
						Vector2 grabPosition = playerPos + playerDir * 100.0f;

						collider->GetRigidBody2D()->SetPosition(grabPosition);

						context.nextStateName = L"Grab";

						context.shouldChangeState = true;

						return;
					}
				}
			}
		}
	}

	if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().heavyAttackStaminaCost &&
		m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval)
	{
		if (context.triggerParams[L"HeavyAttack"])
		{
			context.nextStateName = L"HeavyAttack";

			context.shouldChangeState = true;

			return;
		}
	}

	if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().attackStaminaCost &&
		m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval)
	{
		if (context.triggerParams[L"NormalAttack"])
		{
			context.nextStateName = L"NormalAttack";

			context.shouldChangeState = true;

			return;
		}
	}

	float horizontalInput = context.floatParams[L"HorizontalInput"];
	float verticalInput = context.floatParams[L"VerticalInput"];

	if (horizontalInput != 0.0f || verticalInput != 0.0f)
	{
		context.nextStateName = L"Walk";

		context.shouldChangeState = true;

		return;
	}

	m_player->GetPlayerStatus().currentStamina += m_player->GetPlayerStat().staminaRestoreAmountPerSecond * MyTime::FixedDeltaTime();
	if (m_player->GetPlayerStatus().currentStamina > m_player->GetPlayerStat().maxStamina)
	{
		m_player->GetPlayerStatus().currentStamina = m_player->GetPlayerStat().maxStamina;
	}
}

void PlayerIdleState::Exit(FSMContext& context)
{

}