#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyInteract.h"
#include "EnemyHitAndRun.h"


void EnemyHitAndRun::Enter(FSMContext& context)
{
	m_delayAfterAttack = 1.0f;

	m_didHit = false;
	m_doEvade = false;
}

void EnemyHitAndRun::Update(FSMContext& context)
{
	if (!m_Script) return;	

	if (IsPlayerDead(context))
	{
		IsLockOnTarget() = false;
		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN;
		return;
	}

	if (m_didHit)
	{
		m_delayAfterAttack -= MyTime::DeltaTime();		
	}

	CheckTargetDistance();

	//힛앤런 공격 트리거 1초 후, 또는 플레이어가 최대추적범위 밖으로 나가면 회피! -> 리턴
	if (!m_isEvadeStart && (m_delayAfterAttack <= MyMath::EPSILON) || (!IsTargetInChaseDist()) )
	{
		m_doEvade = true;
	}

	if (m_doEvade)
	{
		m_Script->Evade();
		m_isEvadeStart = true;
		return;
	}

	CheckTargetInAtkRange();
	CheckTargetInAtkAngle();

	if (!IsTargetInAtkAngle())
	{
		m_Script->AheadToTarget();
		return;
	}
		
	if (IsAttackReady() && IsTargetInAtkRange())
	{
		HitAndRunAttack();
		m_didHit = true;
		
	}

	m_didHit = true;

	if (IsPlayerDead(context) || IsPlayerNull())
	{
		IsLockOnTarget() = false;
		IsTargetInRoamDist() = false;
		IsTargetInChaseDist() = false;

		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN; // RETURN 상태로 전환
		return;
	}

}

void EnemyHitAndRun::Exit(FSMContext& context)
{
	MoveSpeed() = 200.0f;
	m_isEvadeStart = false;
	
}


void EnemyHitAndRun::HitAndRunAttack()
{
	float tProb; 

	switch (m_Script->EnemyType())
	{
	case EnemyBase::BASE:
		m_Script->EnemyAttackTypeA_A();
		break;

	case EnemyBase::ZAKO:
		m_Script->EnemyAttackTypeA_A();
		break;

	case EnemyBase::ELITE:
		tProb = Random::Float(0.0f, 1.0f);
		break;

	case EnemyBase::BOSS:
		tProb = Random::Float(0.0f, 1.0f);
		break;
	}
}