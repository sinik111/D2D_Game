#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Script.h"

#include "EnemyOnAttackSecond.h"

void EnemyOnAttackSecond::Enter(FSMContext& context)
{
	m_comboDelay = 0.3f;

	m_Script->IsInCombo() = false;

	m_didAttack = false;

	switch (m_Script->AttackType())
	{
	case 1:
		m_attackTime = m_Script->m_attackA_BTime;
		m_attackComboProb = m_Script->m_attackA_CProb;
		break;
	case 2:
		m_attackTime = m_Script->m_attackB_BTime;
		m_attackComboProb = m_Script->m_attackB_CProb;
		break;
	case 3:
		m_attackTime = m_Script->m_attackC_BTime;
		m_attackComboProb = m_Script->m_attackC_CProb;
		break;

	default:
		break;
	}

}

void EnemyOnAttackSecond::Update(FSMContext& context)
{
	if (!m_Script) return;

	if (m_comboDelay >= MyMath::EPSILON)
	{
		m_comboDelay -= MyTime::DeltaTime();
		return;
	}


	if (m_didAttack && m_attackTime <= MyMath::EPSILON)
	{
		if (m_Script->IsInCombo())
		{
			context.intParams[L"NextEnemyState"] = EnemyBase::ONATTACK3;
			return;
		}
		else
		{
			context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;
			return;
		}
	}

	m_attackTime -= MyTime::DeltaTime();


	if (m_didAttack) return;


	//공격범위 각도 내에 있는지 체크, 콤보니까 거리는 체크 안하고 일단 지름	
	CheckTargetInAtkAngle();
	
	if (!IsTargetInAtkAngle())
	{
		m_Script->AheadToTarget();		
	}

	switch (m_Script->AttackType())
	{
	case 1:
		m_Script->EnemyAttackTypeA_B();
		break;

	case 2:
		m_Script->EnemyAttackTypeB_B();
		break;

	case 3:
		m_Script->EnemyAttackTypeC_B();
		break;
	}

	float t = Random::Float(0.0f, 1.0f);

	if (t <= m_attackComboProb)
	{
		m_Script->IsInCombo() = true;
	}	

	m_didAttack = true;

}

void EnemyOnAttackSecond::Exit(FSMContext& context)
{

}
