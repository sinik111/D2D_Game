#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

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
}

void EnemyEngage::Update(FSMContext& context)
{
	if (!m_Script) return;	

	float distancePlayerToOrigin = Vector2::Distance(Pos(), OriginPos()); // �÷��̾�� ���� ���� �Ÿ�


	//���ݹ��� �ȿ� Ÿ���� �ִ��� üũ. ���ݹ��� �ȿ� Ÿ���� ������ ONATTACK���� ���� ����
	//Enemy�� �˹������ �з���, �÷��̾�� Enemy�� ��ġ�� OriginPos���� MaxChaseDistance �ʸӱ��� ���Դٰ� �ص�, ���ݹ��� �ȿ� ������ �ϴ� ������ �켱
	CheckTargetInAR();

	if (IsTargetInAtkRange())
	{
		//context.intParams[L"NextEnemyState"] = EnemyBase::ONATTACK;
		return;
	}

	//���ݹ��� �ȿ� ���� ���ٸ�, ������ �̵��� �غ�. ���� �Ÿ� üũ
	CheckTargetDistance();

	// �÷��̾� ���, �Ǵ� �����Ÿ� ���̸� IsLockOnTarget = false. ���� ������ ���� ���� ���� ���ε� �����ؾ����� Ȯ��
	if (IsPlayerDead(context) || IsTargetInChaseDist() == false) 
	{
		IsLockOnTarget() = false;
		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN; // RETURN ���·� ��ȯ
		return;
	}

	//������� �Դٸ� Ÿ���� ���� �������� �ȿ� �ִ� ����. ����ؼ� Ÿ���� ���� �̵�	
	MoveToPlayer();

	//UpdateMovement(context);

}

void EnemyEngage::Exit(FSMContext& context)
{
	m_Script->StopMoving();
}

