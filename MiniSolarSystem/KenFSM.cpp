#include "../D2DEngineLib/framework.h"
#include "KenFSM.h"

#include "FSMContext.h"
#include "KenIdleState.h"
#include "KenBackDashState.h"
#include "KenFrontDashState.h"
#include "KenRollState.h"
#include "KenSpinningKickState.h"
#include "KenHurtState.h"
#include "KenDeadState.h"
#include "KenReviveState.h"

KenFSM::KenFSM(FSMContext& context)
{
	m_states.emplace(L"Idle", std::make_unique<KenIdleState>());
	m_states.emplace(L"FrontDash", std::make_unique<KenFrontDashState>());
	m_states.emplace(L"BackDash", std::make_unique<KenBackDashState>());
	m_states.emplace(L"Roll", std::make_unique<KenRollState>());
	m_states.emplace(L"SpinningKick", std::make_unique<KenSpinningKickState>());
	m_states.emplace(L"Hurt", std::make_unique<KenHurtState>());
	m_states.emplace(L"Dead", std::make_unique<KenDeadState>());
	m_states.emplace(L"Revive", std::make_unique<KenReviveState>());

	m_anyStates.push_back(m_states[L"Dead"].get());
	m_anyStates.push_back(m_states[L"Hurt"].get());

	m_currentState = m_states[L"Idle"].get();
	m_currentState->Enter(context);
}

void KenFSM::Update(FSMContext& context)
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
