#pragma once

#include "IState.h"
struct FSMContext;

class KenFSM
{
private:
	std::unordered_map<std::wstring, std::unique_ptr<IState>> m_states;
	std::vector<IState*> m_anyStates;
	IState* m_currentState;

public:
	KenFSM(FSMContext& context);

public:
	void Update(FSMContext& context);
};