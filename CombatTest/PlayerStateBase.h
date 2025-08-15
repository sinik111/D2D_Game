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

public:
	bool m_isStateChanged = false;
	bool m_stateTransitEnd = false;
	
};