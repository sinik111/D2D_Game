#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemyOnAttack.h"

void EnemyOnAttack::Enter(FSMContext& context)
{
	if (IsPlayerNull())
	{
		IsLockOnTarget() = false;
		IsTargetInRoamDist() = false;
		IsTargetInChaseDist() = false;

		return;
	}

	// OnAttack ���� ���� ��, �� �������� true�� ���ۿ� ����
	IsLockOnTarget() = true; 
	IsTargetInChaseDist() = true;
	
	//�÷��̾ ���� ���� �� ����

	
}

void EnemyOnAttack::Update(FSMContext& context)
{
	if (!m_Script) return;

	//�������� �ȸ����� �÷��̾ ���� ȸ������. ȸ���ӵ��� 360�� ȸ���� 1��.

	if (!IsTargetInAtkAngle())
	{
		RotateToTarget();
		return;
	}
	
	if (IsAttackReady())
	{	
		AttackTest(context);
	}		

	// �÷��̾� ���, �Ǵ� �����Ÿ� ���̸� IsLockOnTarget = false. ���� ������ ���� ���� ���� ���ε� �����ؾ����� Ȯ��
	if (IsPlayerDead(context))
	{
		IsLockOnTarget() = false;
		IsTargetInRoamDist() = false;
		IsTargetInChaseDist() = false;

		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN; // RETURN ���·� ��ȯ
		return;
	}

	//���ݹ��� ���� �ִ��� üũ
	CheckTargetInAR();

	//���ݹ��� ���̸� Engage ������Ʈ�� ������ �÷��̾ ���� �̵�
	if(!IsTargetInAtkRange())
		context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;
	
}

void EnemyOnAttack::Exit(FSMContext& context)
{
	
}

void EnemyOnAttack::AttackTest(FSMContext& context)
{
	context.gameObject->GetComponent<EnemyBase>()->FakeAttack();
}