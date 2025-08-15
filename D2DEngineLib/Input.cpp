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

bool Input::IsAnyKeyHeld()
{
	return ComponentSystem::Get().PlayerInput().IsAnyKeyHeld();
}

bool Input::IsAnyKeyPressed()
{
	return ComponentSystem::Get().PlayerInput().IsAnyKeyPressed();
}

bool Input::IsAnyKeyReleased()
{
	return ComponentSystem::Get().PlayerInput().IsAnyKeyReleased();
}

POINT Input::GetCursorPoint()
{
	return ComponentSystem::Get().PlayerInput().GetCursorPoint();
}