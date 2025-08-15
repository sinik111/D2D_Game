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

}

void PlayerDeadState::Update(FSMContext& context)
{
}

void PlayerDeadState::Exit(FSMContext& context)
{
}
