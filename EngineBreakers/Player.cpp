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
#include "../D2DEngineLib/ComponentSystem.h"
#include "../D2DEngineLib/SoundManager.h"

#include "PlayerNormalAttack.h"

#include "PlayerKnockdownCheck.h"
#include "PlayerHeavyAttack.h"

#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerDashState.h"
#include "PlayerEvadeState.h"
#include "PlayerNormalAttackOnceState.h"
#include "PlayerNormalAttackTwiceState.h"
#include "PlayerNormalAttackThreeTimesState.h"
#include "PlayerHeavyAttackState.h"
#include "PlayerHeavyAttackTwiceState.h"

#include "PlayerGrabAttackState.h"
#include "PlayerJumpState.h"
#include "PlayerJumpDownState.h"

#include "PlayerKnockbackState.h"
#include "PlayerKnockdownState.h"
#include "PlayerDeadState.h"
#include "PlayerCameraController.h"
#include "HitStopController.h"

#include "EnemyBase.h"
#include "EnemyBaseAttack.h"

#include "GameDataManager.h"
#include "EnemyBoss.h"
#include "GameOverController.h"



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


	LoadJson(L"StatData/PlayerStat.json");


	GameDataManager::Get().SetPlayerMaxHp(m_playerStat.maxHp);
	GameDataManager::Get().SetPlayerMaxStamina(m_playerStat.maxStamina);
}

void Player::Start()
{
	auto playerInput = GetGameObject()->GetComponent<PlayerInput>();
	playerInput->RegisterDirectionAction(DirectionInputType::Arrow, this, &Player::SetDirectionInput);
	playerInput->RegisterActionOnKey(VK_LSHIFT, KeyState::Pressed, this, &Player::DashKeyPressed);
	playerInput->RegisterActionOnKey(VK_LSHIFT, KeyState::Released, this, &Player::DashKeyReleased);

	playerInput->RegisterActionOnKey('Z', KeyState::Pressed, this, &Player::NormalAttackKeyPressed);
	playerInput->RegisterActionOnKey('Z', KeyState::Released, this, &Player::NormalAttackKeyReleased);

	playerInput->RegisterActionOnKey('X', KeyState::Pressed, this, &Player::HeavyAttackKeyPressed);
	playerInput->RegisterActionOnKey('X', KeyState::Held, this, &Player::HeavyAttackKeyHeld);
	playerInput->RegisterActionOnKey('X', KeyState::Released, this, &Player::HeavyAttackKeyReleased);

	m_fsmContext.gameObject = GetGameObject();
	m_fsmContext.rigidBody2d = m_rigidBody2d;
	m_fsmContext.animator = m_animator;

	AnimationInit();

	m_fsmContext.bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();

	m_fsmContext.bitmapRenderer->SetSortOrder(1);

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
	m_fsmContext.boolParams[L"JumpingUp"] = false;
	m_fsmContext.boolParams[L"JumpingDown"] = false;

	m_playerFSM.AddState<PlayerIdleState>(L"Idle", false, this);
	m_playerFSM.AddState<PlayerWalkState>(L"Walk", false, this);
	m_playerFSM.AddState<PlayerDashState>(L"Dash", false, this);
	m_playerFSM.AddState<PlayerEvadeState>(L"Evade", false, this);

	m_playerFSM.AddState<PlayerNormalAttackOnceState>(L"NormalAttackOnce", false, this);
	m_playerFSM.AddState<PlayerNormalAttackTwiceState>(L"NormalAttackTwice", false, this);
	m_playerFSM.AddState<PlayerNormalAttackThreeTimesState>(L"NormalAttackThreeTimes", false, this);
	m_playerFSM.AddState<PlayerHeavyAttackState>(L"HeavyAttack", false, this);
	m_playerFSM.AddState<PlayerHeavyAttackTwiceState>(L"HeavyAttackTwice", false, this);
	m_playerFSM.AddState<PlayerGrabAttackState>(L"GrabAttack", false, this);
	m_playerFSM.AddState<PlayerJumpState>(L"JumpUp", false, this);
	m_playerFSM.AddState<PlayerJumpDownState>(L"JumpDown", false, this);

	m_playerFSM.AddState<PlayerKnockbackState>(L"Knockback", true, this);
	m_playerFSM.AddState<PlayerKnockdownState>(L"Knockdown", true, this);
	m_playerFSM.AddState<PlayerDeadState>(L"Dead", true, this);

	m_playerFSM.SetState(L"Idle", m_fsmContext);

	m_fsmContext.animator->Play(m_animPath[2]);
	m_fsmContext.animator->SetPlaySpeed(1.0f);

	//m_playerInfoTextRenderer->SetPivot({ 0.0f, 0.0f });
	//m_playerInfoTextRenderer->SetRectSize({ 800.0f, 1080.0f });

	m_evadeOnlyVelocity = 1560.0f;

	UpdateAttackColliderPosition();

}

void Player::FixedUpdate()
{
	if (ComponentSystem::Get().PlayerInput().GetIsStopped() == true)
	{
		return;
	}
	UpdateRigidbody();

	RegenerateStamina();
}

void Player::Update()
{
	if (m_playerStatus.currentHp <= 0.0f && !m_isGameOver)
	{
		m_isGameOver = true;

		GameObject* gameOverController = CreateGameObject(L"GameOverController");
		auto comp = gameOverController->AddComponent<GameOverController>();
		comp->SetPlayer(this);
	}

	m_currentStateName = m_fsmContext.currentStateName;

	//HitStopUpdate();

	UpdateTimers();

	if (m_currentStateName != L"Knockdown")
	{

		if (m_isReadyGrabAttack)
		{
			Debug::Log("그랩 공격 트리거");
			ChangeStateAndAnim(L"GrabAttack");
		}

		UpdateAttackCombo();

		UpdateAnimDirection();

	}

	if (m_currentStateName == L"Knockdown"
		|| m_currentStateName == L"Dead"
		|| m_currentStateName == L"GrabAttack"
		|| m_currentStateName == L"JumpUp"
		|| m_currentStateName == L"JumpDown")
	{
		m_isCanAttack = false;
	}
	else
	{
		m_isCanAttack = true;
	}


	m_playerFSM.Update(m_fsmContext);

	UpdateAttackColliderPosition();

	UpdateDmg();

	GameDataManager::Get().SetPlayerHp(m_playerStatus.currentHp);
	GameDataManager::Get().SetPlayerStamina(m_playerStatus.currentStamina);
}

void Player::OnTriggerStay(const Collision& collision)
{
	//if (collision.otherGameObject->GetName() == L"EnemyBaseAttack")
	//{
	//	auto comp = collision.otherGameObject->GetComponent<EnemyBaseAttack>();

	//	if (comp->GetAttackState() == AttackState::Dealing)
	//	{
	//		comp->SetAttackState(AttackState::Safety);

	//		int enemyAttackPowerMin = 5; // EnemyAttackPower 임시 수치
	//		int enemyAttackPowerMax = 7;

	//		//m_playerStatus.currentHp -= Random::Int(enemyAttackPowerMin, enemyAttackPowerMax);

	//		if (m_playerStatus.currentHp <= 0)
	//		{
	//			m_playerStatus.currentHp = 0;

	//			// state -> dead
	//		}


	//		m_playerStatus.knockdownResetTimer = 0.0f;

	//		int enemyAttackKnockdownPower = 10; // EnemyAttackKnockdownPower 임시 수치

	//		//m_playerStatus.currentKnockdownAmount += enemyAttackKnockdownPower;

	//		if (m_playerStatus.currentKnockdownAmount > m_playerStat.knockdownResist)
	//		{
	//			m_fsmContext.triggerParams[L"Knockdown"] = true;
	//		}
	//		else if (m_playerStatus.currentKnockdownAmount > m_playerStat.knockbackResist)
	//		{
	//			m_fsmContext.triggerParams[L"Knockback"] = true;
	//		}

	//	}
	//}
}

void Player::SetDirectionInput(Vector2 input)
{
	m_fsmContext.floatParams[L"HorizontalInput"] = input.x;
	m_fsmContext.floatParams[L"VerticalInput"] = input.y;

	if (m_onAttack || m_onForceMove || m_fsmContext.currentStateName == L"GrabAttack" || m_fsmContext.currentStateName == L"Knockdown") return;
	if (m_fsmContext.currentStateName == L"JumpUp" || m_fsmContext.currentStateName == L"JumpDown") return;

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

void Player::NormalAttackKeyPressed()
{
	if (m_isColliding)
	{
		Jump();
		return;
	}
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

void Player::Jump()
{
	if (m_isJumptoUP) { JumpUp(); }
	else { JumpDown(); }

	GetGameObject()->GetComponent<RigidBody2D>()->SetPosition(m_landPos);
}

void Player::JumpUp()
{
	m_isJump = true;
	m_fsmContext.boolParams[L"JumpingUp"] = true;
}

void Player::JumpDown()
{
	m_isJump = true;
	m_fsmContext.boolParams[L"JumpingDown"] = true;
}

void Player::WalktoJump()
{
	GetGameObject()->GetComponent<CircleCollider>()->SetTrigger(true);
	m_fsmContext.intParams[L"PlayerDirection"] = m_JumpStartDirectionIndex;
	//lerp(Start, dest, t)
	//m_rigidBody2d->SetVelocity()
}

void Player::Footstep1()
{
	SoundManager::Get().PlaySound(L"PlayerFootstep1", SoundType::SFX);
}

void Player::Footstep2()
{
	SoundManager::Get().PlaySound(L"PlayerFootstep2", SoundType::SFX);
}

void Player::AttackTry1()
{
	SoundManager::Get().PlaySound(L"PlayerAttackTry1", SoundType::SFX);
}

void Player::AttackTry23()
{
	SoundManager::Get().PlaySound(L"PlayerAttackTry23", SoundType::SFX);
}

void Player::AttackSuccess()
{
	
}

void Player::Damaged()
{

}

void Player::DashIn()
{

}

void Player::Dashing()
{

}

void Player::DashEnd()
{
}

void Player::JumpEvent()
{
	SoundManager::Get().PlaySound(L"PlayerJump", SoundType::SFX);
}

void Player::UpdateTimers()
{
	m_playerStatus.evadeIntervalTimer += MyTime::DeltaTime();
	if (m_playerStatus.evadeIntervalTimer >= m_playerStat.evadeInterval)
	{
		m_playerStatus.evadeIntervalTimer = m_playerStat.evadeInterval;
	}

	if (!m_onAttack)
	{
		m_playerStatus.attackIntervalTimer += MyTime::DeltaTime();
		if (m_playerStatus.attackIntervalTimer >= m_playerStat.attackInterval)
		{
			m_playerStatus.attackIntervalTimer = m_playerStat.attackInterval;
		}
	}

	if (m_playerStatus.currentKnockdownAmount > 0)
	{
		m_playerStatus.knockdownResetTimer += MyTime::DeltaTime();
		if (m_playerStatus.knockdownResetTimer >= m_playerStat.knockdownResetTime)
		{
			m_playerStatus.knockdownResetTimer = 0.0f;

			//m_playerStatus.knockdownResetTimer = m_playerStat.knockdownResetTime;
			m_playerStatus.currentKnockdownAmount = 0;
		}
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

Vector2 Player::CalculateDirectionVector(PlayerDirection direction)
{
	return CalculateDirectionVector(static_cast<int>(direction));
}

Vector2 Player::CalculateDirectionVector(int direction)
{
	return s_directionVectors[direction];
}

void Player::SetPlayerNextState(const std::wstring& nextState)
{
	m_fsmContext.nextStateName = nextState;
	m_fsmContext.shouldChangeState = true;
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

	if (m_fsmContext.currentStateName == L"GrabAttack")
	{

	}
	else if (m_isKnockback)
	{
		Vector2 directionVector = m_KnockDir;

		Vector2 desiredVelocity = directionVector * m_currPlayerSpeed;

		m_fsmContext.rigidBody2d->SetVelocity(desiredVelocity);

	}
	else
	{
		if (m_currentStateName == L"NormalAttack" || m_currentStateName == L"NormalAttackOnce" ||
			m_currentStateName == L"NormalAttackTwice" || m_currentStateName == L"NormalAttackThreeTimes"
			|| m_currentStateName == L"HeavyAttack")
		{
			Vector2 directionVector = m_attackDirection;

			Vector2 desiredVelocity = directionVector * m_currPlayerSpeed;

			m_fsmContext.rigidBody2d->SetVelocity(desiredVelocity);

			return;
		}
		else
		{
			//실제이동부분--------------------------------------------------------------------------
			Vector2 directionVector = Player::CalculateDirectionVector(m_movingAheadDirection);

			if (m_movingAheadDirection == 5)
			{
				directionVector = Player::CalculateDirectionVector(m_fsmContext.intParams[L"PlayerDirection"]);
			}

			Vector2 desiredVelocity = directionVector * m_currPlayerSpeed;

			m_fsmContext.rigidBody2d->SetVelocity(desiredVelocity);

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

	m_normalAttackColliderPos = tLocalPos;
}

void Player::AnimationInit()
{
	InitAnimPath(m_idleAnimPath, L"Idle");
	InitAnimPath(m_walkIngAnimPath, L"WalkIng");
	InitAnimPath(m_walkInAnimPath, L"WalkIn");
	InitAnimPath(m_walkEndAnimPath, L"WalkEnd");
	InitAnimPath(m_dashEndAnimPath, L"DashEnd");
	InitAnimPath(m_dashInAnimPath, L"DashIn");
	InitAnimPath(m_dashIngAnimPath, L"DashIng");
	InitAnimPath(m_normalAttackOnceAnimPath, L"NormalAttackOnce");
	InitAnimPath(m_normalAttackTwiceAnimPath, L"NormalAttackTwice");
	InitAnimPath(m_normalAttackThreeTimesAnimPath, L"NormalAttackThreeTimes");
	InitAnimPath(m_heavyAttackAnimPath, L"HeavyAttack");
	InitAnimPath(m_heavyAttackTwiceAnimPath, L"NormalAttackAll");
	//InitAnimPath(m_heavyAttackTwiceAnimPath, L"HeavyAttackTwice");
	InitAnimPath(m_normalAttackAnimPath, L"NormalAttack");
	InitAnimPath(m_normalAttackAllAnimPath, L"NormalAttackAll");
	InitAnimPath(m_grabAttackAnimPath, L"GrabAttack");

	InitAnimPath(m_jumpUpAnimPath, L"JumpUp");
	InitAnimPath(m_jumpDownAnimPath, L"JumpDown");

	InitAnimPath(m_knockdownAnimPath, L"KnockDown");

	SetAnimState(PlayerAnimIdle);

	m_animator->AddActionOnEvent(L"Footstep1", this, &Player::Footstep1);
	m_animator->AddActionOnEvent(L"Footstep2", this, &Player::Footstep2);
	m_animator->AddActionOnEvent(L"AttackTry1st", this, &Player::AttackTry1);
	m_animator->AddActionOnEvent(L"AttackTry2nd3rd", this, &Player::AttackTry23);
	m_animator->AddActionOnEvent(L"DashIn", this, &Player::DashIn);
	m_animator->AddActionOnEvent(L"DashIng", this, &Player::Dashing);
	m_animator->AddActionOnEvent(L"DashIngEnd", this, &Player::DashEnd);
	m_animator->AddActionOnEvent(L"Jump", this, &Player::JumpEvent);
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
	else if (animName == L"JumpUp")
	{
		for (size_t i = 0; i < animPath.size(); ++i)
		{
			if (i == 0 || i == 5)
				continue;

			std::wstring baseFileName = L"JumpUp_" + m_animPathdir[1];

			animPath[i] = baseFileName;
			//animName + L"/" +
			std::wstring spriteSheetPath = L"PlayerAnimation\\JumpUp\\" + baseFileName + L"_sprites.json";
			m_animator->AddSpriteSheet(spriteSheetPath);

			std::wstring animationClipPath = L"PlayerAnimation\\JumpUp\\" + baseFileName + L"_anim.json";
			m_animator->AddAnimationClip(animationClipPath);
		}
	}
	else if (animName == L"NormalAttackOnce")
	{
		std::wstring spriteSheetPath = L"PlayerAnimation\\NormalAttackOnce\\NormalAttackOnce_sprites.json";
		m_animator->AddSpriteSheet(spriteSheetPath);

		for (size_t i = 0; i < animPath.size(); ++i)
		{
			if (i == 0 || i == 5)
				continue;

			std::wstring baseFileName = animName + L"_" + m_animPathdir[i];

			animPath[i] = baseFileName;

			std::wstring animationClipPath = L"PlayerAnimation\\" + animName + L"\\" + baseFileName + L"_anim.json";
			m_animator->AddAnimationClip(animationClipPath);
		}
	}
	else if (animName == L"NormalAttackTwice")
	{
		std::wstring spriteSheetPath = L"PlayerAnimation\\NormalAttackTwice\\NormalAttackTwice_sprites.json";
		m_animator->AddSpriteSheet(spriteSheetPath);

		for (size_t i = 0; i < animPath.size(); ++i)
		{
			if (i == 0 || i == 5)
				continue;

			std::wstring baseFileName = animName + L"_" + m_animPathdir[i];

			animPath[i] = baseFileName;

			std::wstring animationClipPath = L"PlayerAnimation\\" + animName + L"\\" + baseFileName + L"_anim.json";
			m_animator->AddAnimationClip(animationClipPath);
		}
	}
	else if (animName == L"NormalAttackThreeTimes")
	{
		std::wstring spriteSheetPath = L"PlayerAnimation\\NormalAttackThreeTimes\\NormalAttackThreeTimes 1_sprites.json";
		m_animator->AddSpriteSheet(spriteSheetPath);

		for (size_t i = 0; i < animPath.size(); ++i)
		{
			if (i == 0 || i == 5)
				continue;

			std::wstring baseFileName = animName + L" 1_" + m_animPathdir[i];

			animPath[i] = baseFileName;

			std::wstring animationClipPath = L"PlayerAnimation\\" + animName + L"\\" + baseFileName + L"_anim.json";
			m_animator->AddAnimationClip(animationClipPath);
		}
	}
	else if (animName == L"GrabAttack")
	{
		std::wstring spriteSheetPath = L"PlayerAnimation\\GrabAttack\\GrabAttack_sprites.json";
		m_animator->AddSpriteSheet(spriteSheetPath);

		for (size_t i = 0; i < animPath.size(); ++i)
		{
			if (i == 0 || i == 5)
				continue;

			std::wstring baseFileName = animName + L"_" + m_animPathdir[i];

			animPath[i] = baseFileName;

			std::wstring animationClipPath = L"PlayerAnimation\\" + animName + L"\\" + baseFileName + L"_anim.json";
			m_animator->AddAnimationClip(animationClipPath);
		}
	}
	else if (animName == L"HeavyAttack")
	{
		std::wstring spriteSheetPath = L"PlayerAnimation\\HeavyAttack\\HeavyAttack 1_sprites.json";
		m_animator->AddSpriteSheet(spriteSheetPath);

		for (size_t i = 0; i < animPath.size(); ++i)
		{
			if (i == 0 || i == 5)
				continue;

			std::wstring baseFileName = animName + L" 1_" + m_animPathdir[i];

			animPath[i] = baseFileName;

			std::wstring animationClipPath = L"PlayerAnimation\\" + animName + L"\\" + baseFileName + L"_anim.json";
			m_animator->AddAnimationClip(animationClipPath);
		}
	}
	else if (animName == L"JumpDown")
	{
		for (size_t i = 0; i < animPath.size(); ++i)
		{
			if (i == 0 || i == 5)
				continue;

			std::wstring baseFileName = L"JumpDown_" + m_animPathdir[3];

			animPath[i] = baseFileName;
			//animName + L"/" +
			std::wstring spriteSheetPath = L"PlayerAnimation\\JumpDown\\" + baseFileName + L"_sprites.json";
			m_animator->AddSpriteSheet(spriteSheetPath);

			std::wstring animationClipPath = L"PlayerAnimation\\JumpDown\\" + baseFileName + L"_anim.json";
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

	case PlayerAnimNormalAttackOnce:
		temp = m_normalAttackOnceAnimPath;
		break;

	case PlayerAnimNormalAttackTwice:
		temp = m_normalAttackTwiceAnimPath;
		break;

	case PlayerAnimNormalAttackThreeTimes:
		temp = m_normalAttackThreeTimesAnimPath;
		break;

	case PlayerAnimNormalAttack:
		temp = m_normalAttackAnimPath;
		break;
	case PlayerAnimNormalAttackAll:
		temp = m_normalAttackAllAnimPath;
		break;

	case PlayerAnimHeavyAttack:
		temp = m_heavyAttackAnimPath;
		break;

	case PlayerAnimHeavyAttackTwice:
		temp = m_heavyAttackTwiceAnimPath;
		break;

	case PlayerAnimGrabAttack:
		temp = m_grabAttackAnimPath;
		break;

	case PlayerAnimJumpUp:
		temp = m_jumpUpAnimPath;
		break;
	case PlayerAnimJumpDown:
		temp = m_jumpDownAnimPath;
		break;


	case PlayerAnimKnockdown:
		temp = m_knockdownAnimPath;
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
	if (m_fsmContext.triggerParams[L"NormalAttack"])
	{
		if (this->GetPlayerStatus().currentStamina >= this->GetPlayerStat().attackStaminaCost &&
			this->GetPlayerStatus().attackIntervalTimer >= this->GetPlayerStat().attackInterval)
		{
			CircleCollider circleCollider;
			circleCollider.SetCircle({ m_fsmContext.rigidBody2d->GetPosition(), m_grabDistance });
			circleCollider.SetLayer(CollisionLayer::PlayerAttack);
			circleCollider.CalculateSpatialBounds();

			auto resultColliders = ComponentSystem::Get().Physics().DetectCollisions(&circleCollider);

			for (const auto& collider : resultColliders)
			{
				if (collider->GetGameObject()->GetName() == L"EnemyBase")
				{
					Vector2 playerPos = m_fsmContext.rigidBody2d->GetPosition();
					Vector2 enemyPos = collider->GetRigidBody2D()->GetPosition();
					Vector2 toEnemy = enemyPos - playerPos;

					float distance = toEnemy.Length();
					if (m_grabDistance < distance)
					{
						continue;
					}

					Vector2 playerDir = Player::CalculateDirectionVector(m_fsmContext.intParams[L"PlayerDirection"]).Normalized();
					Vector2 toEnemyDir = toEnemy.Normalized();

					float dotResult = Vector2::Dot(playerDir, toEnemyDir);
					float angle = std::acosf(dotResult) * MyMath::RadToDeg;

					if (angle <= 60.0f)
					{
						auto comp = collider->GetGameObject()->GetComponent<EnemyBase>();

						if (comp->IsKnockdown() == false)
						{
							Debug::Log("넉다운상태가 아님");
							return;
						}

						//이펙트!

						Vector2 grabPosition = playerPos + playerDir * 100.0f;

						collider->GetRigidBody2D()->SetPosition(grabPosition);

						ChangeStateAndAnim(L"Grab");

						return;
					}
				}
			}
		}
	}
}


//State 바꾸는 함수
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
	else if (m_fsmContext.nextStateName == L"HeavyAttack")
	{
		SetAnimState(PlayerAnimHeavyAttack);
	}
	//else if (m_fsmContext.nextStateName == L"HeavyAttackTwice")
	//{
	//	SetAnimState(PlayerAnimHeavyAttackTwice);
	//}
	else if (m_fsmContext.nextStateName == L"NormalAttack")
	{
		SetAnimState(PlayerAnimNormalAttack);
	}
	else if (m_fsmContext.nextStateName == L"NormalAttackAll")
	{
		SetAnimState(PlayerAnimNormalAttackAll);
	}
	else if (m_fsmContext.nextStateName == L"GrabAttack")
	{
		SetAnimState(PlayerAnimDashIng);
	}
	else if (m_fsmContext.nextStateName == L"JumpUp")
	{
		SetAnimState(PlayerAnimJumpUp);
		//SetAnimState(PlayerAnimWalkIn);
	}
	else if (m_fsmContext.nextStateName == L"JumpDown")
	{
		SetAnimState(PlayerAnimJumpDown);
		//SetAnimState(PlayerAnimWalkIn);
	}
	else if (m_fsmContext.nextStateName == L"Knockback") { SetAnimState(PlayerAnimIdle); }
	else if (m_fsmContext.nextStateName == L"Knockdown") { SetAnimState(PlayerAnimKnockdown); }

	else if (m_fsmContext.nextStateName == L"Dead") { SetAnimState(PlayerAnimIdle); }
	else { SetAnimState(PlayerAnimIdle); }




	//
	//
	//else if (m_fsmContext.nextStateName == L"Grab")		{ SetAnimState(PlayerAnimNormalAttack); }
	//
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


int Player::ConvertVector2ToKeypadDirection(const Vector2& vec)
{


	if (vec.LengthSq() == 0)
	{
		return 5;
	}


	float angle = atan2(vec.y, vec.x);


	if (angle < 0)
	{
		angle += 2 * 3.14159f;
	}


	int index = static_cast<int>((angle + 3.141592f / 8) / (3.141592f / 4)) % 8;


	switch (index)
	{
	case 0: return 6;  // 오른쪽
	case 1: return 9;  // 오른쪽 위
	case 2: return 8;  // 위
	case 3: return 7;  // 왼쪽 위
	case 4: return 4;  // 왼쪽
	case 5: return 1;  // 왼쪽 아래
	case 6: return 2;  // 아래
	case 7: return 3;  // 오른쪽 아래
	default: return 5; // 기본값 (중앙)
	}

}

void Player::AddKnock(int pd, Vector2 pv)
{

	m_gettingKnock += pd;

	m_KnockDir = pv;
}

void Player::UpdateDmg()
{
	if (m_gettingDmg > 0)
	{
		m_playerStatus.currentHp -= m_gettingDmg;
		m_gettingDmg = 0;
		m_isGetDmg = true;
	}

	if (m_playerStatus.currentHp <= 0)
	{
		//ChangeStateAndAnim(L"Dead");
	}

	if (m_gettingKnock > 0)
	{
		m_playerStatus.currentKnockdownAmount += m_gettingKnock;
		m_gettingKnock = 0;
		m_playerStatus.knockdownResetTimer = 0.0f;
		m_isGetDmg = true;
	}

	

	if (m_playerStatus.currentKnockdownAmount >= m_playerStat.knockbackResist && m_isGetDmg && !m_isKnockback)
	{
		m_isKnockback = true;

		if (m_playerStatus.currentKnockdownAmount >= m_playerStat.knockdownResist && m_currentStateName != L"Knockdown")
		{
			Debug::Log("플레이어 넉따운!");
			m_playerStatus.currentKnockdownAmount = 0;
			ChangeStateAndAnim(L"Knockdown");
		}
		else
		{
			SetAnimState(PlayerAnimKnockdown);
		}		
		
		int t = ConvertVector2ToKeypadDirection(m_KnockDir);
		int reverseKeypadDir;
		switch (t)
		{
		case 1: reverseKeypadDir = 9; break;
		case 2: reverseKeypadDir = 8; break;
		case 3: reverseKeypadDir = 7; break;
		case 4: reverseKeypadDir = 6; break;
		case 6: reverseKeypadDir = 4; break;
		case 7: reverseKeypadDir = 3; break;
		case 8: reverseKeypadDir = 2; break;
		case 9: reverseKeypadDir = 1; break;
		default: reverseKeypadDir = 5; break; // 중앙은 반대도 중앙
		}
		m_fsmContext.intParams[L"PlayerDirection"] = reverseKeypadDir;
		
		Debug::Log(m_KnockDir, "  /  ", reverseKeypadDir);

		m_fsmContext.animator->Play(m_animPath[m_fsmContext.intParams[L"PlayerDirection"]]);		
	}

	if (m_isKnockback && m_currentStateName == L"Knockdown")
	{
		m_kTimer += MyTime::DeltaTime();

		if (m_kTimer < 0.3f)
		{
			m_currPlayerSpeed = 700.0f * (1.0f - m_kTimer / 0.4f);
			
		}
		else if (m_kTimer < 0.39f)
		{
			m_currPlayerSpeed = -150.0f;
		}
		else if (m_kTimer >= 0.4f)
		{			
			m_isKnockback = false;
			m_currPlayerSpeed = 0.0f;
			m_rigidBody2d->SetVelocity(Vector2::Zero);
			m_KnockDir = Vector2::Zero;
			m_kTimer = 0.0f;
		}
	}
	else if (m_isKnockback && m_currentStateName != L"Dead")
	{
		m_kTimer += MyTime::DeltaTime();

		if (m_kTimer < 0.2f)
		{
			m_currPlayerSpeed = 1200.0f * (1.0f - m_kTimer / 0.35f);

		}
		else if (m_kTimer < 0.24f)
		{
			m_fsmContext.animator->Play(m_animPath[m_fsmContext.intParams[L"PlayerDirection"]], 2);	
			m_currPlayerSpeed = 300.0f;
		}
		else if (m_kTimer >= 0.35f)
		{
			SetAnimState(PlayerAnimIdle);
			m_isKnockback = false;
			m_currPlayerSpeed = 0.0f;
			m_rigidBody2d->SetVelocity(Vector2::Zero);
			m_KnockDir = Vector2::Zero;
			m_kTimer = 0.0f;
		}
	}

	m_isGetDmg = false;
}

void Player::UpdateAttackCombo()
{
	if (m_attackPressedCheck != m_isAttackPressed && m_isAttackPressed)
	{
		m_attackPressedCheck = true;


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



void Player::PlayerNormalAttackOnce(float duration)
{
	int directionID = m_fsmContext.intParams[L"PlayerDirection"];

	GetPlayerStatus().currentStamina -= GetPlayerStat().attackStaminaCost;

	m_attackDirection = Player::CalculateDirectionVector(directionID);


	m_attackStartPos = m_rigidBody2d->GetPosition();

	Vector2 destPos = m_attackStartPos;

	destPos += m_attackDirection * 100.0f;

	if (m_isKnockback) {

		m_attackMoveDistSq = 0.01f;

		m_currPlayerSpeed = 0.0f;
	}
	else
	{
		m_attackMoveDistSq = (destPos - m_rigidBody2d->GetPosition()).LengthSq();

		m_currPlayerSpeed = m_attackNormalOnceMoveSpd;
	}

	m_normalAttackGo = this->CreateGameObject(L"PlayerNormalAttack");
	m_normalAttackGo->GetTransform()->SetLocalPosition(m_normalAttackColliderPos);
	m_normalAttackGo->AddComponent<PlayerNormalAttack>(this, duration, 210.0f, 30.0f, m_attack1_1SafetyTime, m_attack1_1EndTime);


	//m_checkKnockdownkGo = this->CreateGameObject(L"PlayerCheckKnockdown");
	//m_checkKnockdownkGo->GetTransform()->SetLocalPosition(m_normalAttackColliderPos);
	//m_checkKnockdownkGo->AddComponent<PlayerKnockdownCheck>(this, 300.0f, 50.0f);

}

void Player::PlayerNormalAttackTwice(float duration)
{

	int directionID = m_fsmContext.intParams[L"PlayerDirection"];

	GetPlayerStatus().currentStamina -= GetPlayerStat().attackStaminaCost;

	m_attackDirection = Player::CalculateDirectionVector(directionID);

	m_attackStartPos = m_rigidBody2d->GetPosition();

	Vector2 destPos = m_attackStartPos;

	destPos += m_attackDirection * 80.0f;

	if (m_isKnockback) {

		m_attackMoveDistSq = 0.01f;

		m_currPlayerSpeed = 0.0f;

	}
	else
	{
		m_attackMoveDistSq = (destPos - m_rigidBody2d->GetPosition()).LengthSq();

		m_currPlayerSpeed = m_attackNormalTwiceMoveSpd;
	}

	m_normalAttackGo = this->CreateGameObject(L"PlayerNormalAttack");
	m_normalAttackGo->GetTransform()->SetLocalPosition(m_normalAttackColliderPos);
	m_normalAttackGo->AddComponent<PlayerNormalAttack>(this, duration, 200.0f, 30.0f, m_attack1_2SafetyTime, m_attack1_2EndTime);

	//m_checkKnockdownkGo = this->CreateGameObject(L"PlayerCheckKnockdown");
	//m_checkKnockdownkGo->GetTransform()->SetLocalPosition(m_normalAttackColliderPos);
	//m_checkKnockdownkGo->AddComponent<PlayerKnockdownCheck>(this, 300.0f, 50.0f);

}

void Player::PlayerNormalAttackThreeTimes(float duration)
{

	int directionID = m_fsmContext.intParams[L"PlayerDirection"];

	GetPlayerStatus().currentStamina -= GetPlayerStat().attackStaminaCost;

	m_attackDirection = Player::CalculateDirectionVector(directionID);

	m_attackStartPos = m_rigidBody2d->GetPosition();

	Vector2 destPos = m_attackStartPos;

	destPos += m_attackDirection * 50.0f;

	if (m_isKnockback) {

		m_attackMoveDistSq = 0.01f;

		m_currPlayerSpeed = 0.0f;
	}
	else
	{
		m_attackMoveDistSq = (destPos - m_rigidBody2d->GetPosition()).LengthSq();

		m_currPlayerSpeed = m_attackNormalThreeTimesMoveSpd;
	}

	m_normalAttackGo = this->CreateGameObject(L"PlayerNormalAttack");
	m_normalAttackGo->GetTransform()->SetLocalPosition(m_normalAttackColliderPos);
	m_normalAttackGo->AddComponent<PlayerNormalAttack>(this, duration, 220.0f, 30.0f, m_attack1_3SafetyTime, m_attack1_3EndTime);

	//m_checkKnockdownkGo = this->CreateGameObject(L"PlayerCheckKnockdown");
	//m_checkKnockdownkGo->GetTransform()->SetLocalPosition(m_normalAttackColliderPos);
	//m_checkKnockdownkGo->AddComponent<PlayerKnockdownCheck>(this, 300.0f, 50.0f);
}

void Player::PlayerHeavyAttack1(float duration)
{
	int directionID = m_fsmContext.intParams[L"PlayerDirection"];

	GetPlayerStatus().currentStamina -= GetPlayerStat().attackStaminaCost;

	m_attackDirection = Player::CalculateDirectionVector(directionID);

	m_heavyAttackGo = this->CreateGameObject(L"PlayerHeavyAttack");
	m_heavyAttackGo->GetTransform()->SetLocalPosition(m_normalAttackColliderPos);
	m_heavyAttackGo->AddComponent<PlayerHeavyAttack>(this, duration, 250.0f, 50.0f, 0.4f, 0.98f);

}


void Player::HitStopUpdate()
{
	if (m_attackHit)
	{
		auto controller = Camera::s_mainCamera->GetGameObject()->GetComponent<HitStopController>();
		controller->DoHitStop(0.01f, 0.1f);

		m_hitStopTimer += MyTime::UnscaledDeltaTime();

		if (m_hitStopTimer >= m_hitStopDuration)
		{
			m_attackHit = false;
			m_hitStopTimer = 0.0f;
		}
	}

	if (m_isOnForceHitStop)
	{
		m_hitStopTimer += MyTime::UnscaledDeltaTime();
		if (m_hitStopTimer >= m_forceHitStopDuration)
			m_hitStopTimer = 0.0f;
		m_forceHitStopDuration = 0.0f;
	}
}

void Player::ForceHitStop(float scale, float duration)
{
	m_forceHitStopDuration = duration;
	auto controller = Camera::s_mainCamera->GetGameObject()->GetComponent<HitStopController>();
	controller->DoHitStop(scale, m_forceHitStopDuration);

	m_isOnForceHitStop = true;

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

void Player::SetIsGameOver(bool over)
{
	m_isGameOver = over;
}

void Player::LoadJson(std::wstring path)
{
	std::ifstream inFile(ResourceManager::Get().GetResourcePath() + path);
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

void Player::SetPlayerInfoTextRenderer(TextRenderer* textRenderer)
{
	m_playerInfoTextRenderer = textRenderer;
}


