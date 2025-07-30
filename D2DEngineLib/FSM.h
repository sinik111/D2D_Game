#pragma once

#include "IState.h"

struct FSMContext;

class FSM
{
private:
	std::unordered_map<std::wstring, std::unique_ptr<IState>> m_states;
	std::vector<IState*> m_anyStates;
	IState* m_currentState = nullptr;

public:
	template<typename T, typename...Args>
	void AddState(const std::wstring& name, bool isAnyState, Args&&... args)
	{
		static_assert(std::is_base_of<IState, T>::value, "State가 아닙니다");

		assert(m_states.find(name) == m_states.end() && "이미 있는 State");

		m_states.emplace(name, std::make_unique<T>(std::forward<Args>(args)...));
		if (isAnyState)
		{
			m_anyStates.push_back(m_states[name].get());
		}
	}

	void RemoveState(const std::wstring& name);

	void Update(FSMContext& context);

	void SetState(const std::wstring& name, FSMContext& context);
};