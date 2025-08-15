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
	if (IsPlayerNull() || IsPlayerDead(context))
	{
		IsLockOnTarget() = false;
		IsTargetInRoamDist() = false;
		IsTargetInChaseDist() = false;

		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN;

		return;
	}
	
	m_Script->StopMoving();
	Debug::Log("스탑무빙");

	MoveSpeed() = 300.0f;

	//기본 m_moveSpeed = 200.0f;
}


void EnemyOnEvade::Update(FSMContext& context)
{
	if (!m_isEvadeStart)
	{
		Debug::Log("회피발동");
		m_Script->Evade();
		m_isEvadeStart = true;
		return;
	}

	if (IsPlayerDead(context) || (!IsTargetInChaseDist() && !IsTargetInMaxAtkRange()))
	{
		IsLockOnTarget() = false;
		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN; 
		return;
	}

	//if (m_Script->m_evadeDistance < Vector2::Distance(Pos(), TargetPos()))
	//{
	//	m_Script->StopMoving();
	//}

	if (!ToDoMove() && m_isEvadeStart)
	{
		m_Script->AheadToTarget();
		context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE; 
	}
}

void EnemyOnEvade::Exit(FSMContext& context)
{
	MoveSpeed() = 200.0f;
	m_isEvadeStart = false;
}