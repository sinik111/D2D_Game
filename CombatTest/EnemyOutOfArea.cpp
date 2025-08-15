#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include <iostream>

#include "EnemyOutOfArea.h"


void EnemyOutOfArea::Enter(FSMContext& context)
{
	if (m_Script->IsDead()) return;

	m_isStateChanged = false;
	IsLockOnTarget() = false;
	IsTargetInChaseDist() = false;
	IsTargetInRoamDist() = false;
	IsUnderAttack() = false;
	IsFindPlayer() = false;
	m_Script->StopMoving();
	m_Script->SetAngleByDirection(Direction());
}

void EnemyOutOfArea::Update(FSMContext& context) 
{		
	if (m_Script->IsDead()) return;

	if (m_isStateChanged) return;	

	if (IsInCamera())
	{ 
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::INAREA; 
	}

	if (Vector2::Distance(Pos(), OriginPos()) > MaxRoamDistance())
	{
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN;
	}

}

void EnemyOutOfArea::Exit(FSMContext& context)
{

}