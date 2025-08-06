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

	if (IsPlayerDead(context) || IsPlayerNull())
	{
		IsLockOnTarget() = false;
		IsTargetInRoamDist() = false;
		IsTargetInChaseDist() = false;

		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN; // RETURN ���·� ��ȯ
		return;
	}

	if (m_attackTime <= MyMath::EPSILON)
	{
		if (m_Script->IsInCombo())
		{
			context.intParams[L"NextEnemyState"] = EnemyBase::ONATTACK2;
			return;
		}
		else
		{
			Debug::Log("�޺� ���̾����� ���� ����");
			context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;
			return;
		}
	}
	
	m_attackTime -= MyTime::DeltaTime();
	Debug::Log(m_attackTime);

	if (m_didAttack) return;
		
	CheckTargetInAtkRange();
	CheckTargetInAtkAngle();

	if (!IsTargetInAtkAngle())
	{
		m_Script->AheadToTarget();
	}	
	
	// �����غ� �Ϸ� ��, ���� Ÿ�� �ֻ��� ������
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
		
		IsAttackReady() = false;
		m_didAttack = true;		
	}
	else
	{		
		context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;
		return;
	}

}

void EnemyOnAttack::Exit(FSMContext& context)
{
	
}


