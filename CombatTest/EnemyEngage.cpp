#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Random.h"

#include <cmath>    
#include <random>   
#include <chrono>   
#include <thread>  

#include "EnemyEngage.h"

void EnemyEngage::Enter(FSMContext& context)
{
	Debug::Log("인게이지 되긴 함");
	m_isStateChanged = false;

	// Enter 시에 타겟이 유효한지 체크. 이후 지형에 따른 접근 가능 여부도 보강해야할지 확인

	if (m_Script != nullptr)
	{
		m_Script->AheadToTarget();
	}	
}

void EnemyEngage::Update(FSMContext& context)
{
	if (!m_Script) return;

	if (m_isStateChanged) return;

	CheckKnockdown(context);

	CheckTargetDistance();	
	CheckTargetInAtkRange();

	if (IsPlayerDead(context) || (!IsTargetInChaseDist()))
	{
		IsLockOnTarget() = false;
		m_Script->StopMoving();
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::EVADEANDRUN;
		return;
	}

	MoveToPlayer();

	if (IsTargetInAtkRange())
	{
		m_Script->StopMoving();

		if (!IsAttackReady())
			return;		

		m_Script->SetAttackType(IsKnockback(), IsTargetInRoamDist());

		if ((IsTargetInChaseDist() && !IsTargetInRoamDist()))
		{			
			m_isStateChanged = true;
			context.intParams[L"NextEnemyState"] = EnemyBase::HITANDRUN;			
			return;
		}		

		if (m_Script->AttackType() == 4)
		{			
			IsAttackReady() = false;
			m_isStateChanged = true;
			context.intParams[L"NextEnemyState"] = EnemyBase::ONEVADE;
			return;
		}

		m_isStateChanged = true;		
		context.intParams[L"NextEnemyState"] = EnemyBase::ONATTACK;
		return;

	}

}

void EnemyEngage::Exit(FSMContext& context)
{
	Debug::Log("인게이지 끝~~");
}

