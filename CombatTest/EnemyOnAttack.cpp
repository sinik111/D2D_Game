#include "../D2DEngineLib/framework.h"

#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Script.h"

#include "EnemyOnAttack.h"
#include "TempEnemyAttack.h"

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
	IsTargetInAtkAngle() = true;	

	m_Script->StopMoving();
}

void EnemyOnAttack::Update(FSMContext& context)
{
	if (!m_Script) return;

	//���� ��ٿ� üũ�� EnemyBaseŬ������ ������Ʈ����, �ٸ� ������Ʈ������ ��ٿ��� ������
	//AttackCoolCheck();

	//���ݹ��� ���� �ִ��� üũ
	CheckTargetInAtkRange();
	CheckTargetInAtkAngle();

	//�ִ���ݹ��� ���̸� Engage ������Ʈ�� ������ �÷��̾ ���� �̵�	
	if (!IsTargetInMaxAtkRange())
	{
		context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;
	}

	if (!IsTargetInAtkAngle())
	{
		m_Script->AheadToTarget();
		return;
	}

	// �����غ� �Ϸ� ��, �� 20% Ȯ���� Evade
	if (IsAttackReady() && IsTargetInAtkRange())
	{				
		if (!IsKnockback())
		{
			float temp = Random::Float(0.0f, 1.0f);
			
			if (temp < m_Script->EvadeProbability())
			{				
				context.intParams[L"NextEnemyState"] = EnemyBase::ONEVADE;
				IsAttackReady() = false;
				return;
			}
		}

		AttackTest();
	}
	
	if (IsPlayerDead(context) || IsPlayerNull())
	{
		IsLockOnTarget() = false;
		IsTargetInRoamDist() = false;
		IsTargetInChaseDist() = false;

		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN; // RETURN ���·� ��ȯ
		return;
	}	
	
}

void EnemyOnAttack::Exit(FSMContext& context)
{
	
}


void EnemyOnAttack::AttackTest()
{	
	auto enemyAttack = m_Script->CreateGameObject(L"EnemyAttackTest");
	auto attackColliderPosition = m_Script->GetGameObject()->GetTransform()->GetLocalPosition();
	attackColliderPosition += (m_Script->m_aheadDirection * 60.0f);

	enemyAttack->GetTransform()->SetLocalPosition(attackColliderPosition);

	auto comp = enemyAttack->AddComponent<TempEnemyAttack>(m_Script);				
	comp->SetDirection(Vector2::EllipseLeftDown);
	
	auto collider = enemyAttack->AddComponent<ConeCollider2D>();
	collider->SetLayer(CollisionLayer::EnemyAttack);
	collider->SetCone(250.0f * Vector2::EllipseLeftDown.Length(), m_Script->m_aheadDirection, 30.0f);
	collider->SetTrigger(true);
	
	auto rb = enemyAttack->AddComponent<RigidBody2D>();
	rb->SetGravityScale(0.0f);
	

	IsAttackReady() = false;
}

//
//void EnemyOnAttack::AttackTest(FSMContext& context)
//{
//	context.gameObject->GetComponent<EnemyBase>()->FakeAttack();
//}