#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Random.h"

#include <cmath>    
#include <random>   
#include <chrono>   
#include <thread>  

#include "EnemyEngage.h"

void EnemyEngage::Enter(FSMContext& context)
{
	// Enter �ÿ� Ÿ���� ��ȿ���� üũ. ���� ������ ���� ���� ���� ���ε� �����ؾ����� Ȯ��

	CheckTargetDistance();

	if (IsPlayerDead(context) || IsTargetInChaseDist() == false)
	{
		IsLockOnTarget() = false;
		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN;
	}

	if (m_Script != nullptr)
	{
		m_Script->AheadToTarget();
	}
}

void EnemyEngage::Update(FSMContext& context)
{
	if (!m_Script) return;

	CheckTargetDistance();
	CheckTargetInAtkRange();

	if (IsPlayerDead(context) || (!IsTargetInChaseDist()))
	{
		IsLockOnTarget() = false;
		context.intParams[L"NextEnemyState"] = EnemyBase::ONEVADE; 
		return;
	}

	if (!IsKnockback() && !IsTargetInAtkRange())
	{
		MoveToPlayer();
		return;
	}

	if (!IsAttackReady())
		return;

	//������� �Դٸ�, ���� �غ� �Ϸ�

	if ((IsTargetInChaseDist() && !IsTargetInRoamDist()))
	{
		context.intParams[L"NextEnemyState"] = EnemyBase::HITANDRUN;
		return;
	}


	m_Script->SetAttackType(IsKnockback());

	if (m_Script->AttackType() == 4)
	{
		IsAttackReady() = false;
		context.intParams[L"NextEnemyState"] = EnemyBase::ONEVADE;
		return;
	}

	context.intParams[L"NextEnemyState"] = EnemyBase::ONATTACK;
	return;
}

void EnemyEngage::Exit(FSMContext& context)
{

}

