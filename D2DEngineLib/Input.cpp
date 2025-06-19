#include "pch.h"
#include "Input.h"

#include "ComponentSystem.h"

bool Input::IsKeyDown(int vKey)
{
	return ComponentSystem::Get().PlayerInput().IsKeyDown(vKey);
}

bool Input::IsKeyPressed(int vKey)
{
	return ComponentSystem::Get().PlayerInput().IsKeyDown(vKey);
}

bool Input::IsKeyReleased(int vKey)
{
	return ComponentSystem::Get().PlayerInput().IsKeyDown(vKey);
}

POINT Input::GetCursorPoint()
{
	return ComponentSystem::Get().PlayerInput().GetCursorPoint();
}