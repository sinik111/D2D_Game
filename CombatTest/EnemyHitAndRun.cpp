#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyHitAndRun.h"


void EnemyHitAndRun::Enter(FSMContext& context)
{
	m_isStateChanged = false;

	MoveSpeed() = m_Script->m_moveSpeedEvade;

	m_didAttack = false;
	m_didEvade = false;
	
	switch (m_Script->AttackType())
	{
	case 1:
		m_attackTime = m_Script->m_attackA_ATime;
		break;
	case 2:
		m_attackTime = m_Script->m_attackB_ATime;
		break;
	case 3:
		m_attackTime = m_Script->m_attackC_ATime;
		break;
	}

	m_delayAfterEvade = 0.5f;
}

void EnemyHitAndRun::Update(FSMContext& context)
{
	if (!m_Script) return;

	if (m_isStateChanged) return;

	CheckKnockdown(context);

	CheckTargetDistance();
	CheckTargetInAtkRange();	

	if (!m_didEvade)
	{
		m_attackTime -= MyTime::DeltaTime();

		if (!m_didAttack)
		{
			if (!IsTargetInAtkAngle())
			{
				m_Script->AheadToTarget();
				Debug::Log(IsTargetInAtkAngle());
			}

			if (IsAttackReady() && IsTargetInMaxAtkRange())
			{
				m_Script->StopMoving();
				m_Script->m_nextAnimationState = EnemyBase::EnemyAnimAttack001;
				HitAndRunAttack();
				m_didAttack = true;
				return;
			}
		}
	}

	//힛앤런 진입후 공격모션시간만큼 경과 후 후, 또는 플레이어가 최대추적범위 밖으로 나가면 회피! -> 리턴
	if (!m_didEvade && ((m_attackTime <= MyMath::EPSILON) || (!IsTargetInChaseDist())))
	{
		m_Script->m_nextAnimationState = EnemyBase::EnemyAnimIdle;
		m_didEvade = true;
		m_Script->Evade(2);
		return;
	}

	if (m_didEvade && !ToDoMove())
	{
		m_delayAfterEvade -= MyTime::DeltaTime();
		m_Script->AheadToTarget();

		if (m_delayAfterEvade <= MyMath::EPSILON)
		{
			
			MoveSpeed() = m_Script->m_moveSpeedOrigin;
			IsLockOnTarget() = false;
			IsUnderAttack() = false;
			m_isStateChanged = true;
			context.intParams[L"NextEnemyState"] = EnemyBase::RETURN;
			return;
		}
	}	

	if (IsPlayerDead(context) || IsPlayerNull())
	{
		IsLockOnTarget() = false;
		IsTargetInRoamDist() = false;
		IsTargetInChaseDist() = false;
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN; // RETURN 상태로 전환
		return;
	}
}

void EnemyHitAndRun::Exit(FSMContext& context)
{
	MoveSpeed() = m_Script->m_moveSpeedOrigin;
}


void EnemyHitAndRun::HitAndRunAttack()
{
	switch (m_Script->AttackType())
	{
	case 1:
		m_Script->EnemyAttackTypeA_A();
		break;

	case 2:
		m_Script->EnemyAttackTypeB_A();
		break;

	case 3:
		m_Script->EnemyAttackTypeC_A();
		break;
	}
}