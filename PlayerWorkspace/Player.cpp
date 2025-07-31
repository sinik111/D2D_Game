#include "../D2DEngineLib/framework.h"
#include "Player.h"

#include <sstream>

#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerDashState.h"
#include "PlayerEvadeState.h"

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
	L"대시 : 회피 후 L - Shift 유지\n" };

void Player::Initialize()
{
	m_rigidBody2d = GetGameObject()->GetComponent<RigidBody2D>();
}

void Player::Start()
{
	auto bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
	bitmapRenderer->SetSortOrder(1);
	//bitmapRenderer->SetPivot({ 0.5f, 1.0f });

	auto textRenderer = GetGameObject()->GetComponent<TextRenderer>();
	textRenderer->SetSortOrder(2);
	textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
	textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
	textRenderer->SetSpaceType(SpaceType::World);

	m_rigidBody2d->SetGravityScale(0.0f);

	auto playerInput = GetGameObject()->GetComponent<PlayerInput>();
	playerInput->RegisterDirectionAction(DirectionInputType::Arrow, this, &Player::SetDirectionInput);
	playerInput->RegisterActionOnKey(VK_LSHIFT, KeyState::Pressed, this, &Player::DashKeyPressed);
	playerInput->RegisterActionOnKey(VK_LSHIFT, KeyState::Released, this, &Player::DashKeyReleased);

	m_fsmContext.gameObject = GetGameObject();
	m_fsmContext.rigidBody2d = m_rigidBody2d;
	m_fsmContext.textRenderer = textRenderer;
	m_fsmContext.floatParams[L"HorizontalInput"] = 0.0f;
	m_fsmContext.floatParams[L"VerticalInput"] = 0.0f;
	m_fsmContext.intParams[L"PlayerDirection"] = static_cast<int>(PlayerDirection::Down);
	m_fsmContext.boolParams[L"Dash"] = false;
	m_fsmContext.triggerParams[L"Evade"] = false;

	m_playerFSM.AddState<PlayerIdleState>(L"Idle", false, this);
	m_playerFSM.AddState<PlayerWalkState>(L"Walk", false, this);
	m_playerFSM.AddState<PlayerDashState>(L"Dash", false, this);
	m_playerFSM.AddState<PlayerEvadeState>(L"Evade", false, this);

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
	std::wostringstream woss;

	woss << keyInfo << L"\n\n";
	woss << m_playerStat << L"\n\n";
	woss << m_playerStatus;

	m_playerInfoTextRenderer->SetText(woss.str());

	m_playerStatus.evadeIntervalTimer += MyTime::DeltaTime();
	if (m_playerStatus.evadeIntervalTimer > m_playerStat.evadeInterval)
	{
		m_playerStatus.evadeIntervalTimer = m_playerStat.evadeInterval;
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
	woss << L"MaxHp: " << playerStat.maxhp << L"\nMaxStamina: " << playerStat.maxStamina
		<< L"\nStaminaRestoreAmountPerSecond: " << playerStat.staminaRestoreAmountPerSecond
		<< L"\nAttackPowerMin: " << playerStat.attackPowerMin << L"\nAttackPowerMax: " << playerStat.attackPowerMax
		<< L"\nAttackStaminaCost: " << playerStat.attackStaminaCost << L"\nHeavyAttackStaminaCost: "
		<< playerStat.heavyAttackStaminaCost << L"\nAttackInterval: " << playerStat.attackInterval
		<< L"\nKnockbackResist: " << playerStat.knockbackResist << L"\nKnockdownResist: " << playerStat.knockdownResist
		<< L"\nKnockdownResetTime: " << playerStat.knockdownResetTime << L"MoveSpeed: " << playerStat.moveSpeed
		<< L"\nDashSpeed: " << playerStat.dashSpeed << L"\nDashStaminaPerSec: " << playerStat.dashStaminaPerSec
		<< L"\nEvadeDistance: " << playerStat.evadeDistance << L"\nEvadeDuration: " << playerStat.evadeDuration
		<< L"\nEvadeAvailableTime: " << playerStat.evadeAvailableTime << L"\nEvadeInterval: " << playerStat.evadeInterval
		<< L"\nEvadeStamina: " << playerStat.evadeStamina;

	return woss;
}

inline std::wostringstream& operator<<(std::wostringstream& woss, const PlayerStatus& playerStatus)
{
	woss << L"CurrentHp: " << playerStatus.currentHp << L"\nCurrentStamina: " << playerStatus.currentStamina
		<< L"\nAttackIntervalTimer: " << playerStatus.attackIntervalTimer << L"\nCurrentKnockback: "
		<< playerStatus.currentKnockback << L"\nCurrentKnockdown: " << playerStatus.currentKnockdown
		<< L"\nKnockdownResetTimer: " << playerStatus.knockdownResetTimer << L"\nEvadeDurationTimer: "
		<< playerStatus.evadeDurationTimer << L"\nEvadeAvailalbeTimer: " << playerStatus.evadeAvailalbeTimer
		<< L"\nEvadeIntervalTimer: " << playerStatus.evadeIntervalTimer;

	return woss;
}
