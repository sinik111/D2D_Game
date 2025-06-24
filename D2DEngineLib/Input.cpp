#include "pch.h"
#include "Input.h"

#include "ComponentSystem.h"

bool Input::IsKeyHeld(int vKey)
{
	return ComponentSystem::Get().PlayerInput().IsKeyHeld(vKey);
}

bool Input::IsKeyPressed(int vKey)
{
	return ComponentSystem::Get().PlayerInput().IsKeyPressed(vKey);
}

bool Input::IsKeyReleased(int vKey)
{
	return ComponentSystem::Get().PlayerInput().IsKeyReleased(vKey);
}

POINT Input::GetCursorPoint()
{
	return ComponentSystem::Get().PlayerInput().GetCursorPoint();
}