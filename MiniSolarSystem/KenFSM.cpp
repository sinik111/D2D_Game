#include "../D2DEngineLib/framework.h"
#include "KenFSM.h"

#include "KenIdleState.h"
#include "KenBackDashState.h"
#include "KenFrontDashState.h"
#include "KenRollState.h"
#include "KenSpinningKickState.h"

KenFSM::KenFSM()
{
	m_states.emplace(L"Idle", std::make_unique<KenIdleState>());
	m_states.emplace(L"FrontDash", std::make_unique<KenFrontDashState>());
	m_states.emplace(L"BackDash", std::make_unique<KenBackDashState>());
	m_states.emplace(L"Roll", std::make_unique<KenRollState>());
	m_states.emplace(L"SpinningKick", std::make_unique<KenSpinningKickState>());

	m_currentState = { L"Idle", m_states[L"Idle"].get() };
}

void KenFSM::Update(FSMContext& context)
{
	m_currentState.second->Update(context);
}
