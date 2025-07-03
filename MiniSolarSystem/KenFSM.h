#pragma once

#include "IState.h"
struct FSMContext;

class KenFSM
{
private:
	std::unordered_map<std::wstring, std::unique_ptr<IState>> m_states;
	std::pair<std::wstring, IState*> m_currentState;

public:
	KenFSM();

public:
	void Update(FSMContext& context);
};