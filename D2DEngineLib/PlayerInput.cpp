#include "pch.h"
#include "PlayerInput.h"

#include "ComponentSystem.h"

PlayerInput::PlayerInput()
{
	ComponentSystem::Get().PlayerInput().Register(this);

	m_directionActions.resize(2);
	m_actionsOnKeyReleased.resize(256);
	m_actionsOnKeyPressed.resize(256);
	m_actionsOnKeyDown.resize(256);
}

PlayerInput::~PlayerInput()
{
	ComponentSystem::Get().PlayerInput().Unregister(this);
}

void PlayerInput::RegisterActionOnKey(short vKey, InputCheckType checkType, std::function<void()> func)
{
	switch (checkType)
	{
	case InputCheckType::Released:
		m_actionsOnKeyReleased[vKey].push_back(func);
		break;

	case InputCheckType::Pressed:
		m_actionsOnKeyPressed[vKey].push_back(func);
		break;

	case InputCheckType::Down:
		m_actionsOnKeyDown[vKey].push_back(func);
		break;
	}
}

void PlayerInput::UnregisterActionsOnKey(short vKey, InputCheckType checkType)
{
	switch (checkType)
	{
	case InputCheckType::Released:
		m_actionsOnKeyReleased[vKey].clear();
		break;

	case InputCheckType::Pressed:
		m_actionsOnKeyPressed[vKey].clear();
		break;

	case InputCheckType::Down:
		m_actionsOnKeyDown[vKey].clear();
		break;
	}
}

void PlayerInput::RegisterDirectionAction(DirectionInputType type, std::function<void(Vector2)> func)
{
	m_directionInputType = type;

	switch (type)
	{
	case DirectionInputType::WASD:
		m_directionActions[(int)DirectionInputType::WASD] = func;
		break;

	case DirectionInputType::Arrow:
		m_directionActions[(int)DirectionInputType::Arrow] = func;
		break;

	case DirectionInputType::Both:
		m_directionActions[(int)DirectionInputType::WASD] = func;
		m_directionActions[(int)DirectionInputType::Arrow] = func;
		break;
	}
}

void PlayerInput::UnregisterDirectionActions()
{
	m_directionActions.clear();
	m_directionActions.resize(2);
}

Vector2 PlayerInput::GetMousePosition() const
{
	return m_mousePosition;
}

void PlayerInput::CallWASDAction(float horizontal, float vertical)
{
	if (m_directionInputType == DirectionInputType::WASD ||
		m_directionInputType == DirectionInputType::Both)
	{
		if (m_directionActions[(int)DirectionInputType::WASD])
		{
			m_directionActions[(int)DirectionInputType::WASD](Vector2(horizontal, vertical));
		}
	}
}

void PlayerInput::CallArrowAction(float horizontal, float vertical)
{
	if (m_directionInputType == DirectionInputType::Arrow ||
		m_directionInputType == DirectionInputType::Both)
	{
		if (m_directionActions[(int)DirectionInputType::Arrow])
		{
			m_directionActions[(int)DirectionInputType::Arrow](Vector2(horizontal, vertical));
		}
	}
}

void PlayerInput::CallActionOnReleased(short vKey)
{
	for (const auto& action : m_actionsOnKeyReleased[vKey])
	{
		action();
	}
}

void PlayerInput::CallActionOnPressed(short vKey)
{
	for (const auto& action : m_actionsOnKeyPressed[vKey])
	{
		action();
	}
}

void PlayerInput::CallActionOnDown(short vKey)
{
	for (const auto& action : m_actionsOnKeyDown[vKey])
	{
		action();
	}
}

void PlayerInput::SetMousePosition(float x, float y)
{
	m_mousePosition = Vector2(x, y);
}