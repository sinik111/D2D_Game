#pragma once

#include "../D2DEngineLib/IState.h"

class Player;

class PlayerStateBase :
	public IState
{
protected:
	Player* m_player;

protected:
	PlayerStateBase(Player* player);
};