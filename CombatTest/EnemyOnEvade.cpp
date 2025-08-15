#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Script.h"

#include "EnemyOnEvade.h"


void EnemyOnEvade::Enter(FSMContext& context)
{
	m_isStateChanged = false;
		
	m_Script->StopMoving();
	m_isEvadeStart = false;

	MoveSpeed() = m_Script->m_moveSpeedEvade;	
}


void EnemyOnEvade::Update(FSMContext& context)
{
	if (!m_Script) return;

	if (m_isStateChanged) return;

	CheckKnockdown(context);

	CheckTargetDistance();
	CheckTargetInAtkRange();

	if (IsPlayerDead(context))
	{
		IsLockOnTarget() = false;
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN;
		return;
	}

	if (!m_isEvadeStart)
	{
		m_Script->Evade(1);
		m_isEvadeStart = true;
		return;
	}

	if (!ToDoMove() && m_isEvadeStart)
	{		
		IsLockOnTarget() = true;
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::SLOWTURN;
		return;

	}
}

void EnemyOnEvade::Exit(FSMContext& context)
{
	MoveSpeed() = m_Script->m_moveSpeedOrigin;
}