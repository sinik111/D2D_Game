#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyOnAttack.h"

void EnemyOnAttack::Enter(FSMContext& context)
{
	if (IsPlayerNull())
	{
		IsLockOnTarget() = false;
		IsTargetInRoamDist() = false;
		IsTargetInChaseDist() = false;

		return;
	}

	// OnAttack 상태 진입 시, 이 변수들은 true일 수밖에 없음
	IsLockOnTarget() = true; 
	IsTargetInChaseDist() = true;
	
	//플레이어를 향해 공격 축 맞춤

	
}

void EnemyOnAttack::Update(FSMContext& context)
{
	if (!m_Script) return;

	//공격축이 안맞으면 플레이어를 향해 회전부터. 회전속도는 360도 회전에 1초.

	if (!IsTargetInAtkAngle())
	{
		RotateToTarget();
		return;
	}
	
	if (IsAttackReady())
	{	
		AttackTest(context);
	}		

	// 플레이어 사망, 또는 추적거리 밖이면 IsLockOnTarget = false. 이후 지형에 따른 접근 가능 여부도 보강해야할지 확인
	if (IsPlayerDead(context))
	{
		IsLockOnTarget() = false;
		IsTargetInRoamDist() = false;
		IsTargetInChaseDist() = false;

		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN; // RETURN 상태로 전환
		return;
	}

	//공격범위 내에 있는지 체크
	CheckTargetInAR();

	//공격범위 밖이면 Engage 스테이트로 이행해 플레이어를 향해 이동
	if(!IsTargetInAtkRange())
		context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;
	
}

void EnemyOnAttack::Exit(FSMContext& context)
{
	
}

void EnemyOnAttack::AttackTest(FSMContext& context)
{
	context.gameObject->GetComponent<EnemyBase>()->FakeAttack();
}