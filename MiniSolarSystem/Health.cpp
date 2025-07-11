#include "../D2DEngineLib/framework.h"
#include "Health.h"

int Health::GetHp() const
{
	return m_hp;
}

int Health::GetMaxHp() const
{
	return m_maxHp;
}

void Health::SetHp(int hp, int maxHp)
{
	if (m_hp != hp || m_maxHp != maxHp)
	{
		int prevHp = m_hp;
		m_hp = hp;

		m_maxHp = maxHp;

		m_onChangeHp.Invoke(prevHp, m_hp, m_maxHp);
	}
}

void Health::TakeDamage(int value, bool destroy)
{
	int currentHp = std::max<int>(0, m_hp - value);

	SetHp(currentHp, m_maxHp);

	if (destroy && m_hp == 0)
	{
		Destroy(GetGameObject());
	}
}

Delegate<int, int, int>& Health::GetOnChangeHp()
{
	return m_onChangeHp;
}
