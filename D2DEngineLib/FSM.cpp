#include "pch.h"
#include "FSM.h"

#include "FSMContext.h"

void FSM::RemoveState(const std::wstring& name)
{
	m_states.erase(name);
}

void FSM::Update(FSMContext& context)
{
	for (const auto& state : m_anyStates)
	{
		if (state->CheckCondition(context))
		{
			m_currentState->Exit(context);

			m_currentState = state;

			m_currentState->Enter(context);

			break;
		}
	}

	m_currentState->Update(context);

	if (context.shouldChangeState)
	{
		const auto& iter = m_states.find(context.nextStateName);
		assert(iter != m_states.end() && L"없는 state 입니다");

		m_currentState->Exit(context);

		m_currentState = iter->second.get();

		m_currentState->Enter(context);
	}

	context.Reset();
}

void FSM::SetState(const std::wstring& name, FSMContext& context)
{
	context.previousStateName = context.currentStateName;
	m_currentState = m_states[name].get();
	m_currentState->Enter(context);
	context.currentStateName = name;
}
