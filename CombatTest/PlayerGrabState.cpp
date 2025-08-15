#include "../D2DEngineLib/framework.h"
#include "PlayerGrabState.h"

#include "Player.h"

#include "../D2DEngineLib/TextRenderer.h"

PlayerGrabState::PlayerGrabState(Player* player)
	: PlayerStateBase(player)
{
}

void PlayerGrabState::Enter(FSMContext& context)
{
	int direction = context.intParams[L"PlayerDirection"];

	std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Grab");

	context.textRenderer->SetText(stateText);

	context.boolParams[L"Invincibility"] = true;
}

void PlayerGrabState::Update(FSMContext& context)
{
	m_dummyGrabAnimationTimer += MyTime::FixedDeltaTime();
	if (m_dummyGrabAnimationTimer >= m_dummyGrabAnimationDuration)
	{
		m_dummyGrabAnimationTimer = 0.0f;

		m_player->ChangeStateWalkOrIdle();

		//float horizontalInput = context.floatParams[L"HorizontalInput"];
		//float verticalInput = context.floatParams[L"VerticalInput"];

		//if (horizontalInput != 0.0f || verticalInput != 0.0f)
		//{
		//	context.nextStateName = L"Walk";
		//}
		//else
		//{
		//	context.nextStateName = L"Idle";
		//}

		//context.shouldChangeState = true;

		return;
	}
}

void PlayerGrabState::Exit(FSMContext& context)
{
	context.boolParams[L"Invincibility"] = false;
}