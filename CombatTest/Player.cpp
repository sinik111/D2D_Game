#include "../D2DEngineLib/framework.h"
#include "Player.h"

#include <sstream>
#include <fstream>

#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/json.hpp"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/Camera.h"

#include "../D2DEngineLib/CircleCollider.h"
#include "../D2DEngineLib/LineCollider.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/ConeCollider2D.h"
//#include "../D2DEngineLib/ComponentSystem.h"

#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerDashState.h"
#include "PlayerEvadeState.h"
#include "PlayerKnockbackState.h"
#include "PlayerKnockdownState.h"
#include "PlayerHeavyAttackState.h"
#include "PlayerGrabState.h"
#include "PlayerDeadState.h"
#include "PlayerCameraController.h"
#include "HitStopController.h"

#include "PlayerNormalAttack.h"
#include "PlayerNormalAttackState.h"
#include "PlayerNormalAttackOnceState.h"
#include "PlayerNormalAttackTwiceState.h"
#include "PlayerNormalAttackThreeTimesState.h"
#include "PlayerNormalAttackAllState.h"


#include "EnemyBase.h"
#include "EnemyBaseAttack.h"

using nlohmann::json;

constexpr static PlayerDirection s_directionEnums[3][3]{
	{
		PlayerDirection::LeftDown,
		PlayerDirection::Left,
		PlayerDirection::LeftUp
	}, // -1, -1 / -1, 0 / -1, 1
	{
		PlayerDirection::Down,
		PlayerDirection::Max,
		PlayerDirection::Up
	}, // 0, -1 / 0, 0 / 0, 1
	{
		PlayerDirection::RightDown,
		PlayerDirection::Right,
		PlayerDirection::RightUp
	} // 1, -1 / 1, 0 / 1, 1
};

const Vector2 s_directionVectors[10]{
		{ 0.0f, 0.0f },
		{ -0.7071f, -0.3536f },
		Vector2(0.0f, -1.0f) * 0.5f,
		{ 0.7071f, -0.3536f },
		Vector2(-1.0f, 0.0f),
		{ 0.0f, 0.0f },
		Vector2(1.0f, 0.0f),
		{ -0.7071f, 0.3536f },
		Vector2(0.0f, 1.0f) * 0.5f,
		{ 0.7071f, 0.3536f },
};

const wchar_t* keyInfo{ L"이동: 방향키\n"
	L"회피: L - Shift 짧게\n"
	L"대시 : 회피 후 L - Shift 유지\n"
	L"공격 : Z\n" };



void Player::Initialize()
{
	m_rigidBody2d = GetGameObject()->GetComponent<RigidBody2D>();
	m_rigidBody2d->SetGravityScale(0.0f);

	m_boxCollider = GetGameObject()->GetComponent<BoxCollider2D>();
	m_boxCollider->SetLayer(CollisionLayer::PlayerHitBox);
	m_boxCollider->SetSize({ 35.0f, 75.0f });
	m_boxCollider->SetOffset({ 0.0f, 50.0f });
	m_boxCollider->SetTrigger(true);

	m_CircleCollider = GetGameObject()->GetComponent<CircleCollider>();
	m_CircleCollider->SetOffset({ 0.0f, 0.0f });
	m_CircleCollider->SetRadius(30.0f);
	m_CircleCollider->SetLayer(CollisionLayer::PlayerMove);

	m_animator = GetGameObject()->GetComponent<Animator>();

	LoadJson(L"PlayerStat.json");
}

void Player::Start()
{
	auto textRenderer = GetGameObject()->GetComponent<TextRenderer>();
	textRenderer->SetSortOrder(2);
	textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
	textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
	textRenderer->SetSpaceType(SpaceType::World);
	textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));

	auto playerInput = GetGameObject()->GetComponent<PlayerInput>();
	playerInput->RegisterDirectionAction(DirectionInputType::Arrow, this, &Player::SetDirectionInput);
	playerInput->RegisterActionOnKey(VK_LSHIFT, KeyState::Pressed, this, &Player::DashKeyPressed);
	playerInput->RegisterActionOnKey(VK_LSHIFT, KeyState::Released, this, &Player::DashKeyReleased);

	playerInput->RegisterActionOnKey('Z', KeyState::Pressed, this, &Player::NormalAttackKeyPressed);
	playerInput->RegisterActionOnKey('Z', KeyState::Released, this, &Player::NormalAttackKeyReleased);

	playerInput->RegisterActionOnKey('B', KeyState::Pressed, this, &Player::NormalAttackAllKeyPressed);

	playerInput->RegisterActionOnKey('X', KeyState::Pressed, this, &Player::HeavyAttackKeyPressed);
	playerInput->RegisterActionOnKey('X', KeyState::Held, this, &Player::HeavyAttackKeyHeld);
	playerInput->RegisterActionOnKey('X', KeyState::Released, this, &Player::HeavyAttackKeyReleased);

	m_fsmContext.gameObject = GetGameObject();
	m_fsmContext.rigidBody2d = m_rigidBody2d;
	m_fsmContext.textRenderer = textRenderer;
	m_fsmContext.animator = m_animator;

	AnimationInit();

	m_fsmContext.floatParams[L"HorizontalInput"] = 0.0f;
	m_fsmContext.floatParams[L"VerticalInput"] = 0.0f;
	m_fsmContext.intParams[L"PlayerDirection"] = static_cast<int>(PlayerDirection::Down);
	m_fsmContext.intParams[L"PrevPlayerDirection"] = static_cast<int>(PlayerDirection::Max);
	m_fsmContext.boolParams[L"Dash"] = false;
	m_fsmContext.triggerParams[L"Evade"] = false;

	m_fsmContext.triggerParams[L"NormalAttack"] = false;
	m_fsmContext.triggerParams[L"NormalAttackAll"] = false;

	m_fsmContext.triggerParams[L"NormalAttackOnce"] = false;
	m_fsmContext.triggerParams[L"NormalAttackTwice"] = false;
	m_fsmContext.triggerParams[L"NormalAttackThreeTimes"] = false;

	m_fsmContext.triggerParams[L"HeavyAttack"] = false;
	m_fsmContext.triggerParams[L"HeavyAttackTwice"] = false;

	m_fsmContext.triggerParams[L"Knockback"] = false;
	m_fsmContext.triggerParams[L"Knockdown"] = false;
	m_fsmContext.boolParams[L"Invincibility"] = false;

	m_playerFSM.AddState<PlayerIdleState>(L"Idle", false, this);
	m_playerFSM.AddState<PlayerWalkState>(L"Walk", false, this);
	m_playerFSM.AddState<PlayerDashState>(L"Dash", false, this);
	m_playerFSM.AddState<PlayerEvadeState>(L"Evade", false, this);

	m_playerFSM.AddState<PlayerNormalAttackState>(L"NormalAttack", false, this);
	m_playerFSM.AddState<PlayerNormalAttackAllState>(L"NormalAttackAll", false, this);

	m_playerFSM.AddState<PlayerNormalAttackOnceState>(L"NormalAttackOnce", false, this);
	m_playerFSM.AddState<PlayerNormalAttackTwiceState>(L"NormalAttackTwice", false, this);
	m_playerFSM.AddState<PlayerNormalAttackThreeTimesState>(L"NormalAttackThreeTimes", false, this);

	m_playerFSM.AddState<PlayerHeavyAttackState>(L"HeavyAttack", false, this);
	m_playerFSM.AddState<PlayerHeavyAttackTwiceState>(L"HeavyAttackTwice", false, this);

	m_playerFSM.AddState<PlayerKnockbackState>(L"Knockback", true, this);
	m_playerFSM.AddState<PlayerKnockdownState>(L"Knockdown", true, this);
	m_playerFSM.AddState<PlayerGrabState>(L"Grab", false, this);
	m_playerFSM.AddState<PlayerDeadState>(L"Dead", true, this);

	m_playerFSM.SetState(L"Idle", m_fsmContext);

	m_fsmContext.animator->Play(m_animPath[2]);
	m_fsmContext.animator->SetPlaySpeed(1.0f);

	m_playerInfoTextRenderer->SetPivot({ 0.0f, 0.0f });
	m_playerInfoTextRenderer->SetRectSize({ 800.0f, 1080.0f });

	m_evadeOnlyVelocity = 1560.0f;

}

void Player::FixedUpdate()
{
	UpdateRigidbody();

	RegenerateStamina();
}

void Player::Update()
{
	m_currentStateName = m_fsmContext.currentStateName;

	UpdateAttackCombo();


	if (m_currentStateName == L"Knockdown" || m_currentStateName == L"Dead" || m_currentStateName == L"Grab")
	{
		m_isCanAttack = false;
	}
	else
	{
		m_isCanAttack = true;
	}


	m_playerFSM.Update(m_fsmContext);

	UpdateAnimDirection();

	UpdateAttackColliderPosition();


	if (Input::IsKeyPressed('1'))
	{
		auto controller = Camera::s_mainCamera->GetGameObject()->GetComponent<PlayerCameraController>();
		controller->ShakeCamera(CameraShakeType::LeftRight, 20.0f, 1.0f, 2, true);
		m_onHoverMode = !m_onHoverMode;
	}

	if (Input::IsKeyPressed('2'))
	{
		auto controller = Camera::s_mainCamera->GetGameObject()->GetComponent<PlayerCameraController>();
		controller->ShakeCamera(CameraShakeType::UpDown, 20.0f, 1.0f, 10, true);
	}

	if (Input::IsKeyPressed('3'))
	{
		auto controller = Camera::s_mainCamera->GetGameObject()->GetComponent<PlayerCameraController>();
		controller->ShakeCamera(CameraShakeType::Random, 20.0f, 3.0f, 1000, true);
	}

	if (Input::IsKeyPressed('4'))
	{
		auto controller = Camera::s_mainCamera->GetGameObject()->GetComponent<HitStopController>();
		controller->DoHitStop(0.01f, 0.15f);
	}


	std::wostringstream woss;

	woss << keyInfo << L"\n\n";
	woss << m_playerStat << L"\n\n";
	woss << m_playerStatus;

	m_playerInfoTextRenderer->SetText(woss.str());

	UpdateTimers();
}

void Player::OnTriggerStay(const Collision& collision)
{
	if (collision.otherGameObject->GetName() == L"EnemyBaseAttack")
	{
		auto comp = collision.otherGameObject->GetComponent<EnemyBaseAttack>();

		if (comp->GetAttackState() == AttackState::Dealing)
		{
			comp->SetAttackState(AttackState::Safety);

			int enemyAttackPowerMin = 5; // EnemyAttackPower 임시 수치
			int enemyAttackPowerMax = 7;

			//m_playerStatus.currentHp -= Random::Int(enemyAttackPowerMin, enemyAttackPowerMax);

			if (m_playerStatus.currentHp <= 0)
			{
				m_playerStatus.currentHp = 0;

				// state -> dead
			}


			m_playerStatus.knockdownResetTimer = 0.0f;

			int enemyAttackKnockdownPower = 10; // EnemyAttackKnockdownPower 임시 수치

			//m_playerStatus.currentKnockdownAmount += enemyAttackKnockdownPower;

			if (m_playerStatus.currentKnockdownAmount > m_playerStat.knockdownResist)
			{
				m_fsmContext.triggerParams[L"Knockdown"] = true;
			}
			else if (m_playerStatus.currentKnockdownAmount > m_playerStat.knockbackResist)
			{
				m_fsmContext.triggerParams[L"Knockback"] = true;
			}

		}
	}
}

void Player::SetDirectionInput(Vector2 input)
{
	m_fsmContext.floatParams[L"HorizontalInput"] = input.x;
	m_fsmContext.floatParams[L"VerticalInput"] = input.y;

	if (m_onAttack || m_onForceMove) return;

	if (input.x != 0.0f || input.y != 0.0f)
	{
		int indexX = static_cast<int>(input.x) + 1;
		int indexY = static_cast<int>(input.y) + 1;

		PlayerDirection direction = s_directionEnums[indexX][indexY];

		m_drag = -1.0f;

		m_fsmContext.intParams[L"PlayerDirection"] = static_cast<int>(direction);

		m_movingAheadDirection = m_fsmContext.intParams[L"PlayerDirection"];
	}
	else
	{
		m_movingAheadDirection = 5;
		m_drag = 0.1f;
	}
}

void Player::DashKeyPressed()
{
	m_fsmContext.boolParams[L"Dash"] = true;

	m_fsmContext.triggerParams[L"Evade"] = true;
}

void Player::DashKeyReleased()
{
	m_fsmContext.boolParams[L"Dash"] = false;
}

void Player::NormalAttackAllKeyPressed()
{
	m_fsmContext.triggerParams[L"NormalAttackAll"] = true;
	Debug::Log("b누름");
}

void Player::NormalAttackKeyPressed()
{
	if (!m_isCanAttack) return;

	m_isAttackPressed = true;
}

void Player::NormalAttackKeyReleased()
{
	m_isAttackPressed = false;
	m_attackPressedCheck = false;
}

void Player::HeavyAttackKeyPressed()
{
	if (!m_isCanAttack) return;

	if (m_fsmContext.currentStateName == L"HeavyAttackTwice") return;

	m_fsmContext.triggerParams[L"HeavyAttack"] = true;
}

void Player::HeavyAttackKeyHeld()
{
	if (!m_isCanAttack)
	{
		m_isReadyHeavyAttackTwice = false;
		return;
	}

	if (m_isOnHeavyAttackOnce)
		m_isReadyHeavyAttackTwice = true;
}

void Player::HeavyAttackKeyReleased()
{
	m_isReadyHeavyAttackTwice = false;
}

void Player::UpdateTimers()
{
	m_playerStatus.evadeIntervalTimer += MyTime::DeltaTime();
	if (m_playerStatus.evadeIntervalTimer >= m_playerStat.evadeInterval)
	{
		m_playerStatus.evadeIntervalTimer = m_playerStat.evadeInterval;
	}

	m_playerStatus.attackIntervalTimer += MyTime::DeltaTime();
	if (m_playerStatus.attackIntervalTimer >= m_playerStat.attackInterval)
	{
		m_playerStatus.attackIntervalTimer = m_playerStat.attackInterval;
	}

	m_playerStatus.knockdownResetTimer += MyTime::DeltaTime();
	if (m_playerStatus.knockdownResetTimer >= m_playerStat.knockdownResetTime)
	{
		m_playerStatus.knockdownResetTimer = m_playerStat.knockdownResetTime;
		m_playerStatus.currentKnockdownAmount = 0;
	}
}



const PlayerStat& Player::GetPlayerStat() const
{
	return m_playerStat;
}

PlayerStatus& Player::GetPlayerStatus()
{
	return m_playerStatus;
}

void Player::SetPlayerInfoTextRenderer(TextRenderer* textRenderer)
{
	m_playerInfoTextRenderer = textRenderer;
}

Vector2 Player::CalculateDirectionVector(PlayerDirection direction)
{
	return CalculateDirectionVector(static_cast<int>(direction));
}

Vector2 Player::CalculateDirectionVector(int direction)
{
	return s_directionVectors[direction];
}

inline std::wostringstream& operator<<(std::wostringstream& woss, const PlayerStat& playerStat)
{
	woss << L"MaxHp: " << playerStat.maxHp << L"\nMaxStamina: " << playerStat.maxStamina
		<< L"\nStaminaRestoreAmountPerSecond: " << playerStat.staminaRestoreAmountPerSecond
		<< L"\nAttackPowerMin: " << playerStat.attackPowerMin << L"\nAttackPowerMax: " << playerStat.attackPowerMax
		<< L"\nHeavyAttackPowerMin: " << playerStat.heavyAttackPowerMin << L"\nHeavyAttackPowerMax: " << playerStat.heavyAttackPowerMax
		<< L"\nGrabAttackPowerMin: " << playerStat.grabAttackPowerMin << L"\nGrabAttackPowerMax: " << playerStat.grabAttackPowerMax
		<< L"\nAttackStaminaCost: " << playerStat.attackStaminaCost << L"\nHeavyAttackStaminaCost: " << playerStat.heavyAttackStaminaCost
		<< L"\nAttackKnockdownPower: " << playerStat.attackKnockdownPower << L"\nHeavyAttackKnockdownPower: " << playerStat.heavyAttackKnockdownPower
		<< L"\nAttackInterval: " << playerStat.attackInterval
		<< L"\nKnockbackResist: " << playerStat.knockbackResist << L"\nKnockdownResist: " << playerStat.knockdownResist
		<< L"\nKnockdownResetTime: " << playerStat.knockdownResetTime << L"\nMoveSpeed: " << playerStat.moveSpeed
		<< L"\nDashSpeed: " << playerStat.dashSpeed << L"\nDashStaminaPerSec: " << playerStat.dashStaminaPerSec
		<< L"\nEvadeDistance: " << playerStat.evadeDistance << L"\nEvadeDuration: " << playerStat.evadeDuration
		<< L"\nEvadeAvailableTime: " << playerStat.evadeAvailableTime << L"\nEvadeInterval: " << playerStat.evadeInterval
		<< L"\nEvadeStamina: " << playerStat.evadeStamina;

	return woss;
}

inline std::wostringstream& operator<<(std::wostringstream& woss, const PlayerStatus& playerStatus)
{
	woss << L"CurrentHp: " << playerStatus.currentHp << L"\nCurrentStamina: " << playerStatus.currentStamina
		<< L"\nAttackIntervalTimer: " << playerStatus.attackIntervalTimer << L"\nCurrentKnockdownAmount: "
		<< playerStatus.currentKnockdownAmount
		<< L"\nKnockdownResetTimer: " << playerStatus.knockdownResetTimer << L"\nEvadeDurationTimer: "
		<< playerStatus.evadeDurationTimer << L"\nEvadeAvailalbeTimer: " << playerStatus.evadeAvailalbeTimer
		<< L"\nEvadeIntervalTimer: " << playerStatus.evadeIntervalTimer;

	return woss;
}

void Player::UpdateRigidbody()
{
	m_debugLogTimer += MyTime::FixedDeltaTime();

	if (m_onHoverMode)
	{
		Vector2 steeringForce;

		if (m_movingAheadDirection != 5 && m_movingAheadDirection != 0)
		{
			Vector2 directionVector = Player::CalculateDirectionVector(m_movingAheadDirection);

			Vector2 desiredVelocity = directionVector * m_playerStat.moveSpeed;

			Vector2 currentVelocity = m_fsmContext.rigidBody2d->GetVelocity();

			m_aheadDirctionVectorMove = currentVelocity;

			if (m_aheadDirctionVectorMove != Vector2::Zero)
			{
				m_aheadDirctionVectorMove.Normalize();
			}
			else
			{
				m_aheadDirctionVectorMove = CalculateDirectionVector(m_fsmContext.intParams[L"PlayerDirection"]);
			}

			const float maxSteeringForce = 1000.0f;

			// 입력이 있을 때: 원하는 속도(desiredVelocity)를 향하도록 가속
			if ((desiredVelocity - currentVelocity).LengthSq() <= 0.1f)
			{
				m_acceleration = 0.0f;
				steeringForce = Vector2::Zero;
			}
			else
			{
				m_acceleration = 1500.0f;
				steeringForce = (desiredVelocity - currentVelocity) * m_acceleration;

				if (steeringForce.LengthSq() > maxSteeringForce * maxSteeringForce)
				{
					steeringForce = steeringForce.Normalized() * maxSteeringForce;

					m_fsmContext.rigidBody2d->AddForce(steeringForce);
				}
			}
		}
		else
		{
			// 입력이 없을 때
			steeringForce = Vector2::Zero;
			m_acceleration = 0.0f; // 가속도는 0으로 설정하여 미끄러지지 않도록 함
		}

		if (m_drag > 0.0f)
		{
			Vector2 currentVelocity = m_fsmContext.rigidBody2d->GetVelocity();

			// 드래그 힘 계산: 현재 속도에 비례하는 반대 방향 힘
			Vector2 dragForce = -currentVelocity * m_drag;

			if (currentVelocity.LengthSq() < 0.01f * 0.01f && steeringForce.LengthSq() <= MyMath::EPSILON)
			{
				m_fsmContext.rigidBody2d->SetVelocity(Vector2::Zero);
				m_drag = -1.0f;
				return;
			}

			// 프레임 시간에 맞춰 감속 적용
			m_fsmContext.rigidBody2d->AddForce(dragForce);
		}
	}
	else
	{
		if (m_currentStateName == L"NormalAttack" || m_currentStateName == L"NormalAttackOnce" ||
			m_currentStateName == L"NormalAttackTwice" || m_currentStateName == L"NormalAttackThreeTimes")
		{
			Vector2 directionVector = attackDirection;

			Vector2 desiredVelocity = directionVector * m_currPlayerSpeed;

			m_fsmContext.rigidBody2d->SetVelocity(desiredVelocity);

			return;
		}

		else
		{
			Vector2 directionVector = Player::CalculateDirectionVector(m_movingAheadDirection);

			if (m_movingAheadDirection == 5)
			{
				directionVector = Player::CalculateDirectionVector(m_fsmContext.intParams[L"PlayerDirection"]);
			}

			Vector2 desiredVelocity = directionVector * m_currPlayerSpeed;

			m_fsmContext.rigidBody2d->SetVelocity(desiredVelocity);
			if (m_debugLogTimer > 1.0f)
			{
				m_debugLogTimer = 0.0f;
				Debug::Log(L" - ", m_rigidBody2d->GetVelocity().Length());
			}

			return;
		}

	}

	//m_fsmContext.intParams[L"PlayerDirection"] = static_cast<int>(PlayerDirection::Down);
	//m_fsmContext.intParams[L"PrevPlayerDirection"] = static_cast<int>(PlayerDirection::Max);
}

void Player::UpdateAttackColliderPosition()
{
	Vector2 tLocalPos = this->GetTransform()->GetLocalPosition();
	tLocalPos.y += 60.0f;
	tLocalPos += (attackDirection * 10.0f);

	m_normalAttackColliderPos = tLocalPos;
}

void Player::AnimationInit()
{
	InitAnimPath(m_idleAnimPath, L"Idle");
	InitAnimPath(m_normalAttackAnimPath, L"NormalAttack");
	InitAnimPath(m_normalAttackOnceAnimPath, L"NormalAttackOnce");
	InitAnimPath(m_normalAttackTwiceAnimPath, L"NormalAttackTwice");
	InitAnimPath(m_normalAttackThreeTimesAnimPath, L"NormalAttackThreeTimes");
	InitAnimPath(m_heavyAttackAnimPath, L"NormalAttackThreeTimes");
	
	InitAnimPath(m_walkIngAnimPath, L"WalkIng");
	InitAnimPath(m_walkInAnimPath, L"WalkIn");
	InitAnimPath(m_walkEndAnimPath, L"WalkEnd");
	InitAnimPath(m_knockdownAnimPath, L"KnockDown");
	InitAnimPath(m_dashEndAnimPath, L"DashEnd");
	InitAnimPath(m_dashInAnimPath, L"DashIn");
	InitAnimPath(m_dashIngAnimPath, L"DashIng");

	InitAnimPath(m_normalAttackAllAnimPath, L"NormalAttackAll");
	InitAnimPath(m_heavyAttackTwiceAnimPath, L"NormalAttackAll");

	SetAnimState(PlayerAnimIdle);
}

void Player::InitAnimPath(AnimPathArray& animPath, const std::wstring& animName)
{
	if (animName == L"NormalAttackAll")
	{
		for (size_t i = 0; i < animPath.size(); ++i)
		{
			if (i == 0 || i == 5)
				continue;

			std::wstring baseFileName = L"NormalAttackThreeTimes_" + m_animPathdir[i];

			animPath[i] = baseFileName;
			//animName + L"/" +
			std::wstring spriteSheetPath = L"PlayerAnimation\\NormalAttackThreeTimesAll\\" + baseFileName + L"_sprites.json";
			m_animator->AddSpriteSheet(spriteSheetPath);

			std::wstring animationClipPath = L"PlayerAnimation\\NormalAttackThreeTimesAll\\" + baseFileName + L"_anim.json";
			m_animator->AddAnimationClip(animationClipPath);
		}
	}
	else
	{
		for (size_t i = 0; i < animPath.size(); ++i)
		{
			if (i == 0 || i == 5)
				continue;

			std::wstring baseFileName = animName + L"_" + m_animPathdir[i];

			animPath[i] = baseFileName;
			//animName + L"/" +
			std::wstring spriteSheetPath = L"PlayerAnimation\\" + animName + L"\\" + baseFileName + L"_sprites.json";
			m_animator->AddSpriteSheet(spriteSheetPath);

			std::wstring animationClipPath = L"PlayerAnimation\\" + animName + L"\\" + baseFileName + L"_anim.json";
			m_animator->AddAnimationClip(animationClipPath);
		}
	}
}

void Player::SetAnimState(int animState)
{
	// 참조를 사용하여 메모리 복사 없이 효율적으로 배열에 접근
	AnimPathArray temp = m_idleAnimPath;

	switch (animState)
	{
	case PlayerAnimIdle:
		temp = m_idleAnimPath;
		break;

	case PlayerAnimNormalAttack:
		temp = m_normalAttackAnimPath;
		break;

	case PlayerAnimNormalAttackOnce:
		temp = m_normalAttackOnceAnimPath;
		break;

	case PlayerAnimNormalAttackTwice:
		temp = m_normalAttackTwiceAnimPath;
		break;

	case PlayerAnimNormalAttackThreeTimes:
		temp = m_normalAttackThreeTimesAnimPath;
		break;

	case PlayerAnimNormalAttackAll:
		temp = m_normalAttackAllAnimPath;
		break;

	case PlayerAnimHeavyAttack:
		temp = m_heavyAttackAnimPath;
		break;
	case PlayerAnimHeavyAttackTwice:
		temp = m_normalAttackAllAnimPath;
		break;
	case PlayerAnimKnockdown:
		temp = m_knockdownAnimPath;
		break;
	case PlayerAnimWalkIn:
		temp = m_walkInAnimPath;
		break;
	case PlayerAnimWalkIng:
		temp = m_walkIngAnimPath;
		break;
	case PlayerAnimWalkEnd:
		temp = m_walkEndAnimPath;
		break;
	case PlayerAnimDashIn:
		temp = m_dashInAnimPath;
		break;
	case PlayerAnimDashIng:
		temp = m_dashIngAnimPath;
		break;
	case PlayerAnimDashEnd:
		temp = m_dashEndAnimPath;
		break;
	case PlayerJumpDown:
		temp = m_jumpDownAnimPath;
		break;
	default:
		temp = m_idleAnimPath;
		break;
	}

	for (size_t i = 0; i < m_animPath.size(); ++i)
	{
		if (i == 0 || i == 5)
		{
			// i == 0은 NoDirection, i == 5는 Max이므로 애니메이션 경로를 2번 인덱스로 지정
			m_animPath[i] = temp[2];
			continue;
		}
		m_animPath[i] = temp[i];
	}

	m_fsmContext.animator->Play(m_animPath[m_fsmContext.intParams[L"PlayerDirection"]]);
}


void Player::UpdateAnimDirection()
{
	int direction = m_movingAheadDirection;

	if (direction != 5 &&
		(direction != m_fsmContext.intParams[L"PrevPlayerDirection"]))
	{
		m_fsmContext.intParams[L"PrevPlayerDirection"] = direction;

		size_t currentFrame = m_fsmContext.animator->GetCurrentFrame();

		m_fsmContext.animator->Play(m_animPath[direction], currentFrame);

		m_isComboWait = false;
		m_comboWaitTimer = 0.0f;

	}
}

void Player::CheckGrabAttack()
{
	//if (m_fsmContext.triggerParams[L"NormalAttack"])
	//{
	//	if (this->GetPlayerStatus().currentStamina >= this->GetPlayerStat().attackStaminaCost &&
	//		this->GetPlayerStatus().attackIntervalTimer >= this->GetPlayerStat().attackInterval)
	//	{
	//		CircleCollider circleCollider;
	//		circleCollider.SetCircle({ m_fsmContext.rigidBody2d->GetPosition(), m_grabDistance });
	//		circleCollider.SetLayer(CollisionLayer::PlayerAttack);
	//		circleCollider.CalculateSpatialBounds();

	//		auto resultColliders = ComponentSystem::Get().Physics().DetectCollisions(&circleCollider);

	//		for (const auto& collider : resultColliders)
	//		{
	//			if (collider->GetGameObject()->GetName() == L"EnemyBase")
	//			{
	//				Vector2 playerPos = m_fsmContext.rigidBody2d->GetPosition();
	//				Vector2 enemyPos = collider->GetRigidBody2D()->GetPosition();
	//				Vector2 toEnemy = enemyPos - playerPos;

	//				float distance = toEnemy.Length();
	//				if (m_grabDistance < distance)
	//				{
	//					continue;
	//				}

	//				Vector2 playerDir = Player::CalculateDirectionVector(m_fsmContext.intParams[L"PlayerDirection"]).Normalized();
	//				Vector2 toEnemyDir = toEnemy.Normalized();

	//				float dotResult = Vector2::Dot(playerDir, toEnemyDir);
	//				float angle = std::acosf(dotResult) * MyMath::RadToDeg;

	//				if (angle <= 60.0f)
	//				{
	//					auto comp = collider->GetGameObject()->GetComponent<EnemyBase>();
	//					
	//					if (comp->IsKnockdown() == false)
	//					{
	//						Debug::Log("넉다운상태가 아님");
	//						return;
	//					}						

	//					//이펙트!

	//					Vector2 grabPosition = playerPos + playerDir * 100.0f;

	//					collider->GetRigidBody2D()->SetPosition(grabPosition);

	//					ChangeStateAndAnim(L"Grab");				

	//					return;
	//				}
	//			}
	//		}
	//	}
	//}
}



void Player::ChangeStateAndAnim(const std::wstring& stateName)
{
	m_fsmContext.nextStateName = stateName;

	if (m_fsmContext.nextStateName == L"Idle")
	{
		if (m_fsmContext.currentStateName == L"Walk")
		{
			SetAnimState(PlayerAnimWalkEnd);
		}
		else if (m_fsmContext.currentStateName == L"Dash" || m_fsmContext.previousStateName == L"Evade")
		{
			SetAnimState(PlayerAnimDashEnd);
		}
		else
		{
			SetAnimState(PlayerAnimIdle);
		}
	}
	else if (m_fsmContext.nextStateName == L"Walk")
	{
		if (m_fsmContext.currentStateName == L"Idle")
		{
			SetAnimState(PlayerAnimWalkIn);
		}
		else if (m_fsmContext.currentStateName == L"Dash" || m_fsmContext.currentStateName == L"Evade")
		{
			SetAnimState(PlayerAnimDashEnd);
		}
		else {
			SetAnimState(PlayerAnimWalkIng);
		}
	}
	else if (m_fsmContext.nextStateName == L"Evade")
	{
		SetAnimState(PlayerAnimDashIn);
	}
	else if (m_fsmContext.nextStateName == L"Dash")
	{
		SetAnimState(PlayerAnimDashIn);
	}

	else if (m_fsmContext.nextStateName == L"NormalAttack")
	{
		SetAnimState(PlayerAnimNormalAttack);
	}
	else if (m_fsmContext.nextStateName == L"NormalAttackOnce")
	{
		SetAnimState(PlayerAnimNormalAttackOnce);
	}
	else if (m_fsmContext.nextStateName == L"NormalAttackTwice")
	{
		SetAnimState(PlayerAnimNormalAttackTwice);
	}
	else if (m_fsmContext.nextStateName == L"NormalAttackThreeTimes")
	{
		SetAnimState(PlayerAnimNormalAttackThreeTimes);
	}
	else if (m_fsmContext.nextStateName == L"NormalAttackAll")
	{
		SetAnimState(PlayerAnimNormalAttackAll);
	}
	else if (m_fsmContext.nextStateName == L"HeavyAttack")
	{
		SetAnimState(PlayerAnimHeavyAttack);
	}
	else if (m_fsmContext.nextStateName == L"HeavyAttackTwice")
	{
		SetAnimState(PlayerAnimHeavyAttackTwice);
	}
	else { SetAnimState(PlayerAnimIdle); }




	//else if (m_fsmContext.nextStateName == L"Knockback")	{ SetAnimState(PlayerAnimIdle); }
	//else if (m_fsmContext.nextStateName == L"Knockdown")	{ SetAnimState(PlayerAnimIdle); }
	//else if (m_fsmContext.nextStateName == L"Grab")		{ SetAnimState(PlayerAnimNormalAttack); }
	//else if (m_fsmContext.nextStateName == L"Dead")		{ SetAnimState(PlayerAnimIdle); }
	//else { SetAnimState(PlayerAnimIdle); }

	m_fsmContext.shouldChangeState = true;
}

void Player::RegenerateStamina()
{
	if (this->GetPlayerStatus().currentStamina >= this->GetPlayerStat().maxStamina)
		return;

	if (m_fsmContext.currentStateName == L"Idle" || m_fsmContext.currentStateName == L"Walk")
	{
		GetPlayerStatus().currentStamina +=
			GetPlayerStat().staminaRestoreAmountPerSecond * MyTime::FixedDeltaTime();
	}

	if (GetPlayerStatus().currentStamina >= GetPlayerStat().maxStamina)
	{
		GetPlayerStatus().currentStamina = GetPlayerStat().maxStamina;
	}

}


void Player::UpdateAttackCombo()
{
	if (m_attackPressedCheck != m_isAttackPressed && m_isAttackPressed)
	{
		m_attackPressedCheck = true;

		Debug::Log("공격키 인식");

		if (!m_isComboWait)
		{
			//if (m_currentStateName == L"Idle")
			//{
			//	m_fsmContext.triggerParams[L"NormalAttack"] = true;
			//}
			//else 
			if (m_currentStateName == L"Idle" || m_currentStateName == L"Walk" || m_currentStateName == L"Dash" || m_currentStateName == L"Evade")
			{
				m_fsmContext.triggerParams[L"NormalAttackOnce"] = true;
			}
			if (m_isComboInputAvailable && (m_currentStateName == L"NormalAttack" || m_currentStateName == L"NormalAttackOnce" || m_currentStateName == L"NormalAttackTwice"))
			{
				m_isNextAttackQueued = true;
			}
		}
		else if (m_isComboWait && (m_lastAttackName == L"NormalAttack" || m_lastAttackName == L"NormalAttackOnce"))
		{
			m_fsmContext.triggerParams[L"NormalAttackTwice"] = true;
		}
		else if (m_isComboWait && m_lastAttackName == L"NormalAttackTwice")
		{
			m_fsmContext.triggerParams[L"NormalAttackThreeTimes"] = true;
		}
		else if (m_lastAttackName == L"NormalAttackThreeTimes")
		{
			m_isComboInputAvailable = false;
			m_isNextAttackQueued = false;
			m_isComboWait = false;
		}
	}
}

void Player::PlayerNormalAttack1(float duration)
{
	Debug::Log("어택 노말어택~!");
	int directionID = m_fsmContext.intParams[L"PlayerDirection"];

	//[Text]
	std::wstring stateText = std::to_wstring(directionID) + std::wstring(L" NormalAttack");
	m_fsmContext.textRenderer->SetText(stateText);

	GetPlayerStatus().currentStamina -= GetPlayerStat().attackStaminaCost;

	attackDirection = Player::CalculateDirectionVector(directionID);

	m_attackStartPos = m_rigidBody2d->GetPosition();

	Vector2 destPos = m_attackStartPos;

	destPos += attackDirection * 100.0f;

	m_attackMoveDistSq = (destPos - m_rigidBody2d->GetPosition()).LengthSq();

	m_currPlayerSpeed = 1300.0f;


	auto normalAttackGo = this->CreateGameObject(L"PlayerNormalAttack");
	auto comp = normalAttackGo->AddComponent<PlayerNormalAttack>(duration);
	comp->SetPlayer(this);

	comp->GetTransform()->SetLocalPosition(m_normalAttackColliderPos);

	comp->SetRigidBody(normalAttackGo->AddComponent<RigidBody2D>());
	comp->GetRigidBody()->SetGravityScale(0.0f);

	comp->SetCollider(normalAttackGo->AddComponent<ConeCollider2D>());
	comp->GetCollider()->SetCone(200.0f * attackDirection.Length(), attackDirection, 10.0f);
	comp->GetCollider()->SetTrigger(true);
	comp->GetCollider()->SetLayer(CollisionLayer::PlayerAttack);

	comp->SetDirection(attackDirection);

}


void Player::PlayerNormalAttackOnce(float duration)
{
	Debug::Log("어택 원스~!");
	int directionID = m_fsmContext.intParams[L"PlayerDirection"];

	//[Text]
	std::wstring stateText = std::to_wstring(directionID) + std::wstring(L" NormalAttack1st");
	m_fsmContext.textRenderer->SetText(stateText);

	GetPlayerStatus().currentStamina -= GetPlayerStat().attackStaminaCost;

	attackDirection = Player::CalculateDirectionVector(directionID);

	m_attackStartPos = m_rigidBody2d->GetPosition();

	Vector2 destPos = m_attackStartPos;

	destPos += attackDirection * 100.0f;

	m_attackMoveDistSq = (destPos - m_rigidBody2d->GetPosition()).LengthSq();

	m_currPlayerSpeed = m_attackNormalOnceMoveSpd;


	auto normalAttackGo = this->CreateGameObject(L"PlayerNormalAttack");
	auto comp = normalAttackGo->AddComponent<PlayerNormalAttack>();
	comp->SetPlayer(this);

	comp->GetTransform()->SetLocalPosition(m_normalAttackColliderPos);

	comp->SetRigidBody(normalAttackGo->AddComponent<RigidBody2D>());
	comp->GetRigidBody()->SetGravityScale(0.0f);

	comp->SetCollider(normalAttackGo->AddComponent<ConeCollider2D>());
	comp->GetCollider()->SetCone(200.0f * attackDirection.Length(), attackDirection, 10.0f);
	comp->GetCollider()->SetTrigger(true);
	comp->GetCollider()->SetLayer(CollisionLayer::PlayerAttack);

	comp->SetDirection(attackDirection);

}

void Player::PlayerNormalAttackTwice(float duration)
{
	Debug::Log("어택 트와이스~!");
	int directionID = m_fsmContext.intParams[L"PlayerDirection"];

	//[Text]
	std::wstring stateText = std::to_wstring(directionID) + std::wstring(L" NormalAttack2nd");
	m_fsmContext.textRenderer->SetText(stateText);

	GetPlayerStatus().currentStamina -= GetPlayerStat().attackStaminaCost;

	attackDirection = Player::CalculateDirectionVector(directionID);

	m_attackStartPos = m_rigidBody2d->GetPosition();

	Vector2 destPos = m_attackStartPos;

	destPos += attackDirection * 80.0f;

	m_attackMoveDistSq = (destPos - m_rigidBody2d->GetPosition()).LengthSq();

	m_currPlayerSpeed = m_attackNormalTwiceMoveSpd;


	auto normalAttackGo = this->CreateGameObject(L"PlayerNormalAttack");
	auto comp = normalAttackGo->AddComponent<PlayerNormalAttack>();
	comp->SetPlayer(this);

	comp->GetTransform()->SetLocalPosition(m_normalAttackColliderPos);

	comp->SetRigidBody(normalAttackGo->AddComponent<RigidBody2D>());
	comp->GetRigidBody()->SetGravityScale(0.0f);

	comp->SetCollider(normalAttackGo->AddComponent<ConeCollider2D>());
	comp->GetCollider()->SetCone(200.0f * attackDirection.Length(), attackDirection, 10.0f);
	comp->GetCollider()->SetTrigger(true);
	comp->GetCollider()->SetLayer(CollisionLayer::PlayerAttack);

	comp->SetDirection(attackDirection);

}

void Player::PlayerNormalAttackThreeTimes(float duration)
{
	Debug::Log("어택 쓰리타임즈~!");
	int directionID = m_fsmContext.intParams[L"PlayerDirection"];

	//[Text]
	std::wstring stateText = std::to_wstring(directionID) + std::wstring(L" NormalAttack3rd");
	m_fsmContext.textRenderer->SetText(stateText);

	GetPlayerStatus().currentStamina -= GetPlayerStat().attackStaminaCost;

	attackDirection = Player::CalculateDirectionVector(directionID);

	m_attackStartPos = m_rigidBody2d->GetPosition();

	Vector2 destPos = m_attackStartPos;

	destPos += attackDirection * 50.0f;

	m_attackMoveDistSq = (destPos - m_rigidBody2d->GetPosition()).LengthSq();

	m_currPlayerSpeed = m_attackNormalThreeTimesMoveSpd;


	auto normalAttackGo = this->CreateGameObject(L"PlayerNormalAttack");
	auto comp = normalAttackGo->AddComponent<PlayerNormalAttack>();
	comp->SetPlayer(this);

	comp->GetTransform()->SetLocalPosition(m_normalAttackColliderPos);

	comp->SetRigidBody(normalAttackGo->AddComponent<RigidBody2D>());
	comp->GetRigidBody()->SetGravityScale(0.0f);

	comp->SetCollider(normalAttackGo->AddComponent<ConeCollider2D>());
	comp->GetCollider()->SetCone(200.0f * attackDirection.Length(), attackDirection, 10.0f);
	comp->GetCollider()->SetTrigger(true);
	comp->GetCollider()->SetLayer(CollisionLayer::PlayerAttack);

	comp->SetDirection(attackDirection);

}

void Player::PlayerNormalAttackAll(float duration)
{
	Debug::Log("이것은 단순해보이지만 3가지 기술이 합쳐진 콤비네이션!");

	int directionID = m_fsmContext.intParams[L"PlayerDirection"];

	//[Text]
	std::wstring stateText = std::to_wstring(directionID) + std::wstring(L" NormalAttackAll");
	m_fsmContext.textRenderer->SetText(stateText);

	GetPlayerStatus().currentStamina -= GetPlayerStat().attackStaminaCost;

	attackDirection = Player::CalculateDirectionVector(directionID);

	m_attackStartPos = m_rigidBody2d->GetPosition();

	Vector2 destPos = m_attackStartPos;

	destPos += attackDirection * 250.0f;

	m_attackMoveDistSq = (destPos - m_rigidBody2d->GetPosition()).LengthSq();

	m_currPlayerSpeed = 1300.0f;


	auto normalAttackGo = this->CreateGameObject(L"PlayerNormalAttack");
	auto comp = normalAttackGo->AddComponent<PlayerNormalAttack>();
	comp->SetPlayer(this);

	comp->GetTransform()->SetLocalPosition(m_normalAttackColliderPos);

	comp->SetRigidBody(normalAttackGo->AddComponent<RigidBody2D>());
	comp->GetRigidBody()->SetGravityScale(0.0f);

	comp->SetCollider(normalAttackGo->AddComponent<ConeCollider2D>());
	comp->GetCollider()->SetCone(200.0f * attackDirection.Length(), attackDirection, 10.0f);
	comp->GetCollider()->SetTrigger(true);
	comp->GetCollider()->SetLayer(CollisionLayer::PlayerAttack);

	comp->SetDirection(attackDirection);
}

void Player::PlayerHeavyAttack(float duration)
{
	

	int directionID = m_fsmContext.intParams[L"PlayerDirection"];

	//[Text]
	std::wstring stateText = std::to_wstring(directionID) + std::wstring(L" HeavyAttack");
	m_fsmContext.textRenderer->SetText(stateText);

	GetPlayerStatus().currentStamina -= GetPlayerStat().attackStaminaCost;

	attackDirection = Player::CalculateDirectionVector(directionID);

	m_attackStartPos = m_rigidBody2d->GetPosition();

	Vector2 destPos = m_attackStartPos;

	destPos += attackDirection * 100.0f;

	m_attackMoveDistSq = (destPos - m_rigidBody2d->GetPosition()).LengthSq();

	m_currPlayerSpeed = m_attackHeavyOnceMoveSpd;;


	auto normalAttackGo = this->CreateGameObject(L"PlayerHeavyAttack");
	auto comp = normalAttackGo->AddComponent<PlayerNormalAttack>();
	comp->SetPlayer(this);

	comp->GetTransform()->SetLocalPosition(m_normalAttackColliderPos);

	comp->SetRigidBody(normalAttackGo->AddComponent<RigidBody2D>());
	comp->GetRigidBody()->SetGravityScale(0.0f);

	comp->SetCollider(normalAttackGo->AddComponent<ConeCollider2D>());
	comp->GetCollider()->SetCone(200.0f * attackDirection.Length(), attackDirection, 10.0f);
	comp->GetCollider()->SetTrigger(true);
	comp->GetCollider()->SetLayer(CollisionLayer::PlayerAttack);

	comp->SetDirection(attackDirection);
}



void Player::ChangeStateWalkOrIdle()
{
	if (m_fsmContext.floatParams[L"HorizontalInput"] != 0.0f || m_fsmContext.floatParams[L"VerticalInput"] != 0.0f)
	{
		ChangeStateAndAnim(L"Walk");
	}
	else
	{
		ChangeStateAndAnim(L"Idle");
	}
}

void Player::LoadJson(std::wstring path)
{
	std::ifstream inFile(ResourceManager::Get().GetResourcePath() + L"PlayerStat.json");
	if (inFile.is_open())
	{
		json j;
		inFile >> j;
		inFile.close();

		m_playerStat.maxHp = j["MaxHp"];
		m_playerStat.maxStamina = j["MaxStamina"];
		m_playerStat.staminaRestoreAmountPerSecond = j["StaminaRestoreAmountPerSecond"];
		m_playerStat.attackPowerMin = j["AttackPowerMin"];
		m_playerStat.attackPowerMax = j["AttackPowerMax"];
		m_playerStat.heavyAttackPowerMin = j["HeavyAttackPowerMin"];
		m_playerStat.heavyAttackPowerMax = j["HeavyAttackPowerMax"];
		m_playerStat.grabAttackPowerMin = j["GrabAttackPowerMin"];
		m_playerStat.grabAttackPowerMax = j["GrabAttackPowerMax"];
		m_playerStat.attackStaminaCost = j["AttackStaminaCost"];
		m_playerStat.heavyAttackStaminaCost = j["HeavyAttackStaminaCost"];
		m_playerStat.attackKnockdownPower = j["AttackKnockdownPower"];
		m_playerStat.heavyAttackKnockdownPower = j["HeavyAttackKnockdownPower"];
		m_playerStat.attackInterval = j["AttackInterval"];
		m_playerStat.knockbackResist = j["KnockbackResist"];
		m_playerStat.knockdownResist = j["KnockdownResist"];
		m_playerStat.knockdownResetTime = j["KnockdownResetTime"];
		m_playerStat.moveSpeed = j["MoveSpeed"];
		m_playerStat.dashSpeed = j["DashSpeed"];
		m_playerStat.dashStaminaPerSec = j["DashStaminaPerSec"];
		m_playerStat.evadeDistance = j["EvadeDistance"];
		m_playerStat.evadeDuration = j["EvadeDuration"];
		m_playerStat.evadeAvailableTime = j["EvadeAvailableTime"];
		m_playerStat.evadeInterval = j["EvadeInterval"];
		m_playerStat.evadeStamina = j["EvadeStamina"];
	}
	else
	{
		assert(false && L"json 파일 열기 실패");
	}

	m_playerStatus.currentHp = m_playerStat.maxHp;
	m_playerStatus.currentStamina = m_playerStat.maxStamina;
}

static void from_json(const json& j, PlayerStat& playerStat)
{
	j.at("MaxHp").get_to(playerStat.maxHp);
	j.at("MaxStamina").get_to(playerStat.maxStamina);
	j.at("StaminaRestoreAmountPerSecond").get_to(playerStat.staminaRestoreAmountPerSecond);
	j.at("AttackPowerMin").get_to(playerStat.attackPowerMin);
	j.at("AttackPowerMax").get_to(playerStat.attackPowerMax);
	j.at("HeavyAttackPowerMin").get_to(playerStat.heavyAttackPowerMin);
	j.at("HeavyAttackPowerMax").get_to(playerStat.heavyAttackPowerMax);
	j.at("GrabAttackPowerMin").get_to(playerStat.grabAttackPowerMin);
	j.at("GrabAttackPowerMax").get_to(playerStat.grabAttackPowerMax);
	j.at("AttackStaminaCost").get_to(playerStat.attackStaminaCost);
	j.at("HeavyAttackStaminaCost").get_to(playerStat.heavyAttackStaminaCost);
	j.at("AttackKnockdownPower").get_to(playerStat.attackKnockdownPower);
	j.at("HeavyAttackKnockdownPower").get_to(playerStat.heavyAttackKnockdownPower);
	j.at("AttackInterval").get_to(playerStat.attackInterval);
	j.at("KnockbackResist").get_to(playerStat.knockbackResist);
	j.at("KnockdownResist").get_to(playerStat.knockdownResist);
	j.at("KnockdownResetTime").get_to(playerStat.knockdownResetTime);
	j.at("MoveSpeed").get_to(playerStat.moveSpeed);
	j.at("DashSpeed").get_to(playerStat.dashSpeed);
	j.at("DashStaminaPerSec").get_to(playerStat.dashStaminaPerSec);
	j.at("EvadeDistance").get_to(playerStat.evadeDistance);
	j.at("EvadeDuration").get_to(playerStat.evadeDuration);
	j.at("EvadeAvailableTime").get_to(playerStat.evadeAvailableTime);
	j.at("EvadeInterval").get_to(playerStat.evadeInterval);
	j.at("EvadeStamina").get_to(playerStat.evadeStamina);
}


