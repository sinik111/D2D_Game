#include "../D2DEngineLib/framework.h"
#include "PlayerDeadState.h"

#include "../D2DEngineLib/TextRenderer.h"

#include "Player.h"

PlayerDeadState::PlayerDeadState(Player* player)
    : PlayerStateBase(player)
{
}

bool PlayerDeadState::CheckCondition(FSMContext& context)
{
    return context.currentStateName != L"Dead" && m_player->GetPlayerStatus().currentHp <= 0;
}

void PlayerDeadState::Enter(FSMContext& context)
{
    int direction = context.intParams[L"PlayerDirection"];

    std::wstring stateText = std::to_wstring(direction) + std::wstring(L" Dead");

    context.textRenderer->SetText(stateText);
}

void PlayerDeadState::Update(FSMContext& context)
{
}

void PlayerDeadState::Exit(FSMContext& context)
{
}
