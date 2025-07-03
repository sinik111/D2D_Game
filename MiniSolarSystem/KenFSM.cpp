#include "../D2DEngineLib/framework.h"
#include "KenFSM.h"

#include "FSMContext.h"
#include "KenIdleState.h"
#include "KenBackDashState.h"
#include "KenFrontDashState.h"
#include "KenRollState.h"
#include "KenSpinningKickState.h"

KenFSM::KenFSM(FSMContext& context)
{
	m_states.emplace(L"Idle", std::make_unique<KenIdleState>());
	m_states.emplace(L"FrontDash", std::make_unique<KenFrontDashState>());
	m_states.emplace(L"BackDash", std::make_unique<KenBackDashState>());
	m_states.emplace(L"Roll", std::make_unique<KenRollState>());
	m_states.emplace(L"SpinningKick", std::make_unique<KenSpinningKickState>());

	m_currentState = m_states[L"Idle"].get();
	m_currentState->Enter(context);
}

void KenFSM::Update(FSMContext& context)
{
	m_currentState->Update(context);

	if (context.hasRequest)
	{
		const auto& iter = m_states.find(context.nextStateName);
		assert(iter != m_states.end() && L"없는 state 입니다");

		m_currentState->Exit(context);

		m_currentState = iter->second.get();

		m_currentState->Enter(context);
	}

	context.Reset();
}
