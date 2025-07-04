#include "../D2DEngineLib/framework.h"
#include "KenDeadState.h"

#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/MyTime.h"

#include "FSMContext.h"

bool KenDeadState::CheckCondition(FSMContext& context)
{
	if (context.currentStateName != L"Dead")
	{
		if (context.boolParams[L"IsDead"])
		{
			return true;
		}
	}
	
	return false;
}

void KenDeadState::Enter(FSMContext& context)
{
	context.animator->Play(L"ken_dead");
	context.currentStateName = L"Dead";

	MyTime::SetTimeScale(0.2f);
}

void KenDeadState::Update(FSMContext& context)
{
	if (context.animator->IsFinished() && !m_once)
	{
		MyTime::SetTimeScale(1.0f);
		m_once = true;
	}
}

void KenDeadState::Exit(FSMContext& context)
{
}
