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
	//IsLockOnTarget() = true; // Engage ���� ���� �� ���� ����
	//IsTargetInChaseDist() = true;
	//IsTargetInRoamDist() = true or false;


	// Enter �ÿ� Ÿ���� ��ȿ���� üũ. ���� ������ ���� ���� ���� ���ε� �����ؾ����� Ȯ��
	
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
	
	//���ݹ��� �ȿ� Ÿ���� �ִ��� üũ. ���ݹ��� �ȿ� Ÿ���� ������ ONATTACK���� ���� ����
	//Enemy�� �˹������ �з���, �÷��̾�� Enemy�� ��ġ�� OriginPos���� MaxChaseDistance �ʸӱ��� ���Դٰ� �ص�, ���ݹ��� �ȿ� ������ �ϴ� ������ �켱
	CheckTargetInAtkRange();

	if (IsTargetInAtkRange())
	{		
		context.intParams[L"NextEnemyState"] = EnemyBase::ONATTACK;
		return;
	}
	

	//���ݹ��� �ȿ� ���� ���ٸ�, ������ �̵��� �غ�. ���� �Ÿ� üũ
	CheckTargetDistance();

	// �÷��̾� ���, �Ǵ� �����Ÿ� �� && ���ݻ�Ÿ� ���̶�� IsLockOnTarget = false. 
	// ���� ȸ���ϰ�, ����. 
	if (IsPlayerDead(context) || (!IsTargetInChaseDist() && !IsTargetInMaxAtkRange()) )
	{
		IsLockOnTarget() = false;
		context.intParams[L"NextEnemyState"] = EnemyBase::ONEVADE; // 
		return;
	}

	//������� �Դٸ� Ÿ���� ���� �������� �ȿ� �ִ� ����. ����ؼ� Ÿ���� ���� �̵�	
	//���� �˹���ϰ� �ִ� ���̶�� �̵� ����
	
	if (!m_Script->IsKnockback())
	{
		MoveToPlayer();
	}

	//UpdateMovement(context);

}

void EnemyEngage::Exit(FSMContext& context)
{
	m_Script->StopMoving();
}

