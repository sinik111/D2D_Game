#include "../D2DEngineLib/framework.h"
#include "Player.h"

#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "PlayerDashState.h"
#include "PlayerDodgeState.h"

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
	m_fsmContext.floatParams[L"MoveSpeed"] = m_moveSpeed;
	m_fsmContext.floatParams[L"DashSpeed"] = m_dashSpeed;
	m_fsmContext.floatParams[L"DodgeDistance"] = m_dodgeDistance;
	m_fsmContext.floatParams[L"DodgeSpeed"] = m_dodgeSpeed;
	m_fsmContext.intParams[L"PlayerDirection"] = static_cast<int>(PlayerDirection::Down);
	m_fsmContext.boolParams[L"Dash"] = false;
	m_fsmContext.triggerParams[L"Dodge"] = false;
	m_fsmContext.floatParams[L"DodgeAvailableTime"] = m_dodgeAvailableTime;

	m_playerFSM.AddState<PlayerIdleState>(L"Idle");
	m_playerFSM.AddState<PlayerWalkState>(L"Walk");
	m_playerFSM.AddState<PlayerDashState>(L"Dash");
	m_playerFSM.AddState<PlayerDodgeState>(L"Dodge");

	m_playerFSM.SetState(L"Idle", m_fsmContext);
}

void Player::FixedUpdate()
{
	m_playerFSM.Update(m_fsmContext);
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

	m_fsmContext.triggerParams[L"Dodge"] = true;
}

void Player::DashKeyReleased()
{
	m_fsmContext.boolParams[L"Dash"] = false;
}

Vector2 Player::CalculateDirectionVector(PlayerDirection direction)
{
	return CalculateDirectionVector(static_cast<int>(direction));
}

Vector2 Player::CalculateDirectionVector(int direction)
{
	return s_directionVectors[direction];
}
