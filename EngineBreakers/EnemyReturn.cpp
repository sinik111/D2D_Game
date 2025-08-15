#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyReturn.h"


void EnemyReturn::Enter(FSMContext& context)
{
	m_isStateChanged = false;

	m_Script->StopMoving();
		
	m_Script->MoveTo(OriginPos());
}

void EnemyReturn::Update(FSMContext& context)
{
	if (!m_Script) return;	

	if (m_Script->m_isDead == true || m_Script->m_hp <= 0)
	{
		return;
	}

	if (m_isStateChanged) return;

	CheckKnockdown(context);

	CheckTargetDistance();
	CheckTargetInAtkRange();

	if (IsUnderAttack())
	{		
		m_Script->StopMoving();
	
		IsLockOnTarget() = true;
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::SLOWTURN;
		return;		
	}

	if (!ToDoMove())
	{
		if (!IsPlayerNull())
		{
			m_Script->AheadToTarget();
		}
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::INAREA;
		return;
	}	
}

void EnemyReturn::Exit(FSMContext& context)
{

}

