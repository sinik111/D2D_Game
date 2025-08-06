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

#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerDashState.h"
#include "PlayerEvadeState.h"
#include "PlayerNormalAttackState.h"
#include "EnemyBaseAttack.h"
#include "PlayerKnockbackState.h"
#include "PlayerKnockdownState.h"
#include "PlayerHeavyAttackState.h"
#include "PlayerGrabState.h"
#include "PlayerDeadState.h"
#include "PlayerCameraController.h"
#include "HitStopController.h"

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

const Vector2 s_directionVectors[9]{
		{ -0.7071f, 0.3536f },
		Vector2(0.0f, 1.0f) * 0.5f,
		{ 0.7071f, 0.3536f },
		Vector2(1.0f, 0.0f),
		{ 0.7071f, -0.3536f },
		Vector2(0.0f, -1.0f) * 0.5f,
		{ -0.7071f, -0.3536f },
		Vector2(-1.0f, 0.0f),
		{ 0.0f, 0.0f }
};

const wchar_t* keyInfo{ L"이동: 방향키\n"
	L"회피: L - Shift 짧게\n"
	L"대시 : 회피 후 L - Shift 유지\n"
	L"공격 : Z\n" };

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

void Player::Initialize()
{
	m_rigidBody2d = GetGameObject()->GetComponent<RigidBody2D>();

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

void Player::Start()
{
	auto textRenderer = GetGameObject()->GetComponent<TextRenderer>();
	textRenderer->SetSortOrder(2);
	textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
	textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
	textRenderer->SetSpaceType(SpaceType::World);
	textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));

	m_rigidBody2d->SetGravityScale(0.0f);

	auto playerInput = GetGameObject()->GetComponent<PlayerInput>();
	playerInput->RegisterDirectionAction(DirectionInputType::Arrow, this, &Player::SetDirectionInput);
	playerInput->RegisterActionOnKey(VK_LSHIFT, KeyState::Pressed, this, &Player::DashKeyPressed);
	playerInput->RegisterActionOnKey(VK_LSHIFT, KeyState::Released, this, &Player::DashKeyReleased);
	playerInput->RegisterActionOnKey('Z', KeyState::Pressed, this, &Player::NormalAttackKeyPressed);
	playerInput->RegisterActionOnKey('X', KeyState::Pressed, this, &Player::HeavyAttackKeyPressed);
	playerInput->RegisterActionOnKey('X', KeyState::Held, this, &Player::HeavyAttackKeyHeld);
	playerInput->RegisterActionOnKey('X', KeyState::Released, this, &Player::HeavyAttackKeyReleased);

	m_fsmContext.gameObject = GetGameObject();
	m_fsmContext.rigidBody2d = m_rigidBody2d;
	m_fsmContext.textRenderer = textRenderer;
	m_fsmContext.animator = GetGameObject()->GetComponent<Animator>();
	m_fsmContext.floatParams[L"HorizontalInput"] = 0.0f;
	m_fsmContext.floatParams[L"VerticalInput"] = 0.0f;
	m_fsmContext.intParams[L"PlayerDirection"] = static_cast<int>(PlayerDirection::Down);
	m_fsmContext.intParams[L"PrevPlayerDirection"] = static_cast<int>(PlayerDirection::Max);
	m_fsmContext.boolParams[L"Dash"] = false;
	m_fsmContext.triggerParams[L"Evade"] = false;
	m_fsmContext.triggerParams[L"NormalAttack"] = false;
	m_fsmContext.triggerParams[L"HeavyAttack"] = false;
	m_fsmContext.boolParams[L"ContinueHeavyAttack"] = false;
	m_fsmContext.triggerParams[L"Knockback"] = false;
	m_fsmContext.triggerParams[L"Knockdown"] = false;
	m_fsmContext.boolParams[L"Invincibility"] = false;

	m_fsmContext.animator->Play(L"runs_S_001");
	m_fsmContext.animator->SetPlaySpeed(2.0f);

	m_playerFSM.AddState<PlayerIdleState>(L"Idle", false, this);
	m_playerFSM.AddState<PlayerWalkState>(L"Walk", false, this);
	m_playerFSM.AddState<PlayerDashState>(L"Dash", false, this);
	m_playerFSM.AddState<PlayerEvadeState>(L"Evade", false, this);
	m_playerFSM.AddState<PlayerNormalAttackState>(L"NormalAttack", false, this);
	m_playerFSM.AddState<PlayerHeavyAttackState>(L"HeavyAttack", false, this);
	m_playerFSM.AddState<PlayerKnockbackState>(L"Knockback", true, this);
	m_playerFSM.AddState<PlayerKnockdownState>(L"Knockdown", true, this);
	m_playerFSM.AddState<PlayerGrabState>(L"Grab", false, this);
	m_playerFSM.AddState<PlayerDeadState>(L"Dead", true, this);

	m_playerFSM.SetState(L"Idle", m_fsmContext);

	m_playerInfoTextRenderer->SetPivot({ 0.0f, 0.0f });
	m_playerInfoTextRenderer->SetRectSize({ 800.0f, 1080.0f });
}

void Player::FixedUpdate()
{
	m_playerFSM.Update(m_fsmContext);
}

void Player::Update()
{
	if (Input::IsKeyPressed('1'))
	{
		auto controller = Camera::s_mainCamera->GetGameObject()->GetComponent<PlayerCameraController>();
		controller->ShakeCamera(CameraShakeType::LeftRight, 20.0f, 1.0f, 2, true);
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
		controller->DoHitStop(0.01f, 0.5f);
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

	if (input.x != 0.0f || input.y != 0.0f)
	{
		int indexX = static_cast<int>(input.x) + 1;
		int indexY = static_cast<int>(input.y) + 1;

		PlayerDirection direction = s_directionEnums[indexX][indexY];

		m_fsmContext.intParams[L"PlayerDirection"] = static_cast<int>(direction);
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
	m_fsmContext.triggerParams[L"NormalAttack"] = true;
}

void Player::HeavyAttackKeyPressed()
{
	m_fsmContext.triggerParams[L"HeavyAttack"] = true;
}

void Player::HeavyAttackKeyHeld()
{
	m_fsmContext.boolParams[L"ContinueHeavyAttack"] = true;
}

void Player::HeavyAttackKeyReleased()
{
	m_fsmContext.boolParams[L"ContinueHeavyAttack"] = false;
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
