#include "../D2DEngineLib/framework.h"
#include "PlayerIdleState.h"

#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/RigidBody2D.h"

void PlayerIdleState::Enter(FSMContext& context)
{
	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Idle");

	context.textRenderer->SetText(stateText);
}

void PlayerIdleState::Update(FSMContext& context)
{
	float horizontalInput = context.floatParams[L"HorizontalInput"];
	float verticalInput = context.floatParams[L"VerticalInput"];

	if (horizontalInput != 0.0f || verticalInput != 0.0f)
	{
		context.nextStateName = L"Walk";

		context.shouldChangeState = true;

		return;
	}
}

void PlayerIdleState::Exit(FSMContext& context)
{
}
