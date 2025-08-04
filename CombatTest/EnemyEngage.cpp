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
	//IsLockOnTarget() = true; // Engage 상태 진입 시 락온 시작
	//IsTargetInChaseDist() = true;
	//IsTargetInRoamDist() = true or false;


	// Enter 시에 타겟이 유효한지 체크. 이후 지형에 따른 접근 가능 여부도 보강해야할지 확인
	
	if (IsPlayerDead(context) || IsTargetInChaseDist() == false)
	{
		IsLockOnTarget() = false;
		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN; 
	}

	if (m_Script != nullptr)
	{
		m_Script->AheadToTarget();
	}
}

void EnemyEngage::Update(FSMContext& context)
{
	if (!m_Script) return;	
	
	//공격범위 안에 타겟이 있는지 체크. 공격범위 안에 타겟이 있으면 ONATTACK으로 상태 변경
	//Enemy가 넉백등으로 밀려서, 플레이어와 Enemy의 위치가 OriginPos에서 MaxChaseDistance 너머까지 나왔다고 해도, 공격범위 안에 있으면 일단 공격을 우선
	CheckTargetInAtkRange();

	if (IsTargetInAtkRange())
	{		
		context.intParams[L"NextEnemyState"] = EnemyBase::ONATTACK;
		return;
	}
	

	//공격범위 안에 적이 없다면, 적까지 이동을 준비. 먼저 거리 체크
	CheckTargetDistance();

	// 플레이어 사망, 또는 추적거리 밖 && 공격사거리 밖이라면 IsLockOnTarget = false. 
	// 이후 회피하고, 리턴. 
	if (IsPlayerDead(context) || (!IsTargetInChaseDist() && !IsTargetInMaxAtkRange()) )
	{
		IsLockOnTarget() = false;
		context.intParams[L"NextEnemyState"] = EnemyBase::ONEVADE; // 
		return;
	}

	//여기까지 왔다면 타겟이 아직 추적범위 안에 있는 상태. 계속해서 타겟을 향해 이동	
	//지금 넉백당하고 있는 중이라면 이동 안함
	
	if (!m_Script->IsKnockback())
	{
		MoveToPlayer();
	}

	//UpdateMovement(context);

}

void EnemyEngage::Exit(FSMContext& context)
{
	m_Script->StopMoving();
}

