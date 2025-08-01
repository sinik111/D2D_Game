#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyReturn.h"


void EnemyReturn::Enter(FSMContext& context)
{
	IsLockOnTarget() = false; 
	IsTargetInChaseDist() = false;
	IsTargetInRoamDist() = false;
	IsUnderAttack() = false;
		
	m_Script->MoveTo(OriginPos());
}

void EnemyReturn::Update(FSMContext& context)
{
	if (!m_Script) return;		

	if (IsUnderAttack())
	{		
		m_Script->StopMoving();

		IsLockOnTarget() = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;
		return;		
	}

	if (!ToDoMove())
	{
		context.intParams[L"NextEnemyState"] = EnemyBase::INAREA;
	}	

	
}

void EnemyReturn::Exit(FSMContext& context)
{

}

