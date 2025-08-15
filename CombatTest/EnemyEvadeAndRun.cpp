#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyEvadeAndRun.h"

void EnemyEvadeAndRun::Enter(FSMContext& context)
{
	m_isStateChanged = false;

	MoveSpeed() = m_Script->m_moveSpeedEvade;

	m_didEvade = false;
	
	m_delayAfterEvade = 1.0f;
}



void EnemyEvadeAndRun::Update(FSMContext& context)
{
	if (!m_Script) return;

	if (m_isStateChanged) return;

	CheckKnockdown(context);

	CheckTargetDistance();
	CheckTargetInAtkRange();

	if (!m_didEvade)
	{		
		m_didEvade = true;
		m_Script->m_nextAnimationState = EnemyBase::EnemyAnimIdle;
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
}

void EnemyEvadeAndRun::Exit(FSMContext& context)
{
	MoveSpeed() = m_Script->m_moveSpeedOrigin;
}