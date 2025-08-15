#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Script.h"

#include "EnemyOnAttack.h"

void EnemyOnAttack::Enter(FSMContext& context)
{
	m_isStateChanged = false;

	m_Script->StopMoving();
	m_Script->IsInCombo() = false;

	m_didAttack = false;
	
	switch (m_Script->AttackType())
	{
	case 1:
		m_attackTime = m_Script->m_attackA_ATime;
		m_attackComboProb = m_Script->m_attackA_BProb;
		break;
	case 2:
		m_attackTime = m_Script->m_attackB_ATime;
		m_attackComboProb = m_Script->m_attackB_BProb;
		break;
	case 3:
		m_attackTime = m_Script->m_attackC_ATime;
		m_attackComboProb = m_Script->m_attackC_BProb;
		break;

	default:
		break;
	}
}

void EnemyOnAttack::Update(FSMContext& context)
{
	//AttackCoolCheck();

	if (!m_Script) return;

	if (m_Script->m_isDead == true || m_Script->m_hp <= 0)
	{
		return;
	}

	if (m_isStateChanged) return;

	CheckKnockdown(context);

	if (m_didAttack && m_attackTime <= MyMath::EPSILON)
	{
		if (m_Script->IsInCombo())
		{
			m_isStateChanged = true;
			context.intParams[L"NextEnemyState"] = EnemyBase::ONATTACK2;
			return;
		}

		IsAttackReady() = false;
		m_isStateChanged = true;
		m_Script->IsInCombo() = false;
		context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;
		return;

	}

	m_attackTime -= MyTime::DeltaTime();

	if (m_didAttack) return;

	CheckTargetInAtkRange();
	CheckTargetInAtkAngle();

	if (!IsTargetInAtkAngle())
	{
		m_Script->AheadToTarget();
	}

	// 공격준비 완료 시, 어택 타입 주사위 굴리기
	if (IsTargetInAtkRange())
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

		float t = Random::Float(0.0f, 1.0f);

		if (t <= m_attackComboProb)
		{
			m_Script->IsInCombo() = true;
		}
		else
		{
			m_Script->IsInCombo() = false;
		}
		

		m_didAttack = true;
	}
	else
	{
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;
		return;
	}

}

void EnemyOnAttack::Exit(FSMContext& context)
{

}


