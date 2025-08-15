#include "../D2DEngineLib/framework.h"
#include "PlayerWalkState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/ComponentSystem.h"
#include "../D2DEngineLib/CircleCollider.h"

#include "Player.h"
#include "DummyEnemy.h"

PlayerWalkState::PlayerWalkState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerWalkState::Enter(FSMContext& context)
{
	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Walk");

	context.textRenderer->SetText(stateText);

	switch (static_cast<PlayerDirection>(direction))
	{
	case PlayerDirection::Down:
		context.animator->Play(L"runs_S_001");
		break;
	case PlayerDirection::Left:
		context.animator->Play(L"runs_W_001");
		break;
	case PlayerDirection::LeftDown:
		context.animator->Play(L"runs_SW_001");
		break;
	case PlayerDirection::LeftUp:
		context.animator->Play(L"runs_NW_001");
		break;
	case PlayerDirection::Right:
		context.animator->Play(L"runs_E_001");
		break;
	case PlayerDirection::RightDown:
		context.animator->Play(L"runs_SE_001");
		break;
	case PlayerDirection::RightUp:
		context.animator->Play(L"runs_NE_001");
		break;
	case PlayerDirection::Up:
		context.animator->Play(L"runs_N_001");
		break;
	}
}

void PlayerWalkState::Update(FSMContext& context)
{
	if (context.triggerParams[L"HeavyAttack"] || context.triggerParams[L"NormalAttack"])
	{
		if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().attackStaminaCost &&
			m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval)
		{
			CircleCollider circleCollider;
			circleCollider.SetCircle({ context.rigidBody2d->GetPosition(), m_dummyGrabDistance });
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

	if (context.triggerParams[L"HeavyAttack"])
	{
		if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().heavyAttackStaminaCost &&
			m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval)
		{
			context.nextStateName = L"HeavyAttack";

			context.shouldChangeState = true;

			return;
		}
	}

	if (context.triggerParams[L"NormalAttack"])
	{
		if (m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().attackStaminaCost &&
			m_player->GetPlayerStatus().attackIntervalTimer >= m_player->GetPlayerStat().attackInterval)
		{
			context.nextStateName = L"NormalAttack";

			context.shouldChangeState = true;

			return;
		}
	}

	float horizontalInput = context.floatParams[L"HorizontalInput"];
	float verticalInput = context.floatParams[L"VerticalInput"];

	if (horizontalInput == 0.0f && verticalInput == 0.0f)
	{
		context.nextStateName = L"Idle";

		context.shouldChangeState = true;

		return;
	}

	if (context.triggerParams[L"Evade"])
	{
		m_canDodge = true;
	}

	if (!context.boolParams[L"Dash"])
	{
		if (m_canDodge &&
			m_player->GetPlayerStatus().evadeIntervalTimer >= m_player->GetPlayerStat().evadeInterval &&
			m_player->GetPlayerStatus().currentStamina >= m_player->GetPlayerStat().evadeStamina)
		{
			context.nextStateName = L"Evade";

			context.shouldChangeState = true;

			return;
		}
	}

	m_player->GetPlayerStatus().currentStamina += m_player->GetPlayerStat().staminaRestoreAmountPerSecond * MyTime::FixedDeltaTime();
	if (m_player->GetPlayerStatus().currentStamina > m_player->GetPlayerStat().maxStamina)
	{
		m_player->GetPlayerStatus().currentStamina = m_player->GetPlayerStat().maxStamina;
	}

	if (m_canDodge)
	{
		m_player->GetPlayerStatus().evadeAvailalbeTimer += MyTime::FixedDeltaTime();

		if (m_player->GetPlayerStatus().evadeAvailalbeTimer >= m_player->GetPlayerStat().evadeAvailableTime)
		{
			m_canDodge = false;

			m_player->GetPlayerStatus().evadeAvailalbeTimer = 0.0f;
		}
	}

	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Walk");

	if (direction != context.intParams[L"PrevPlayerDirection"])
	{
		context.intParams[L"PrevPlayerDirection"] = direction;

		size_t currentFrame = context.animator->GetCurrentFrame();

		switch (static_cast<PlayerDirection>(direction))
		{
		case PlayerDirection::Down:
			context.animator->Play(L"runs_S_001", currentFrame);
			break;
		case PlayerDirection::Left:
			context.animator->Play(L"runs_W_001", currentFrame);
			break;
		case PlayerDirection::LeftDown:
			context.animator->Play(L"runs_SW_001", currentFrame);
			break;
		case PlayerDirection::LeftUp:
			context.animator->Play(L"runs_NW_001", currentFrame);
			break;
		case PlayerDirection::Right:
			context.animator->Play(L"runs_E_001", currentFrame);
			break;
		case PlayerDirection::RightDown:
			context.animator->Play(L"runs_SE_001", currentFrame);
			break;
		case PlayerDirection::RightUp:
			context.animator->Play(L"runs_NE_001", currentFrame);
			break;
		case PlayerDirection::Up:
			context.animator->Play(L"runs_N_001", currentFrame);
			break;
		}
	}
	
	context.textRenderer->SetText(stateText);

	Vector2 directionVector = Player::CalculateDirectionVector(direction);

	context.rigidBody2d->SetVelocity(directionVector * m_player->GetPlayerStat().moveSpeed);
}

void PlayerWalkState::Exit(FSMContext& context)
{
	context.rigidBody2d->SetVelocity({ 0.0f, 0.0f });

	m_canDodge = false;

	m_player->GetPlayerStatus().evadeAvailalbeTimer = 0.0f;
}
