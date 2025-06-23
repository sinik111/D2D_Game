#pragma once

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/Delegate.h"

class Health :
	public Script
{
private:
	Delegate<int, int, int> m_onChangeHp; // prev, curr, max
	int m_hp{};
	int m_maxHp{};

public:
	int GetHp() const;
	int GetMaxHp() const;
	void SetHp(int hp, int maxHp);

	void TakeDamage(int value);

	Delegate<int, int, int>& GetOnChangeHp();
};