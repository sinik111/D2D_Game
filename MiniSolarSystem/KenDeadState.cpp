#include "../D2DEngineLib/framework.h"
#include "KenDeadState.h"

#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/MyTime.h"
#include "../D2DEngineLib/GameObject.h"

#include "FSMContext.h"
#include "Health.h"

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

	//MyTime::SetTimeScale(0.2f);

	context.animator->SetPlaySpeed(0.5f);
}

void KenDeadState::Update(FSMContext& context)
{
	if (context.animator->IsFinished())
	{
		if (context.triggerParams[L"Revive"])
		{
			context.shouldChangeState = true;
			context.nextStateName = L"Revive";

			Health* hp = context.gameObject->GetComponent<Health>();
			hp->SetHp(50, 50);
		}
	}
}

void KenDeadState::Exit(FSMContext& context)
{
	context.animator->SetPlaySpeed(1.0f);
}
