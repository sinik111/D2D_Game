#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/BoxCollider2D.h"

#include <cmath>

#include "../D2DEngineLib/FSMContext.h"

#include "EnemyState.h"


void EnemyState::Enter(FSMContext& context)
{


}

void EnemyState::Update(FSMContext& context)
{

}

void EnemyState::Exit(FSMContext& context)
{

}

const bool& EnemyState::IsPlayerDead(FSMContext& context) {
	return context.boolParams[L"IsPlayerDead"];
}



RigidBody2D* EnemyState::RigidBody()
{
	return m_Script->RigidBody();
}


void EnemyState::CheckTargetDistance()
{
	if (!m_Script) return;

	const Vector2 playerPos = TargetPos(); // �÷��̾� ��ġ
	const Vector2 originPos = OriginPos(); // ���� ���� ���� (����)

	float distancePlayerToOrigin = Vector2::Distance(playerPos, originPos); // �÷��̾�� ���� ���� �Ÿ�

	float maxChaseDistance = MaxChaseDistance(); // �߰� �ִ� �Ÿ�
	float maxRoamDistance = MaxRoamDistance();   // �̵� �ִ� �Ÿ� (�������� MaxMoveDistance�� ���)


	IsTargetInRoamDist() = (distancePlayerToOrigin <= maxRoamDistance) ? true : false;

	IsTargetInChaseDist() = (distancePlayerToOrigin <= maxChaseDistance) ? true : false;

}



void EnemyState::MoveToPlayer()
{
	if (!m_Script) return;

	const Vector2 playerPos = TargetPos();
	const Vector2 enemyPos = Pos();

	// �÷��̾�� �ڽ��� ��ġ�� �������� ����
	Vector2 toPlayer = playerPos - enemyPos;
	float distanceToPlayer = toPlayer.Length();

	// ��ǥ ���� ����: �÷��̾�κ��� 10.0f��ŭ ������ ��ġ
	// ���� ���͸� ����ȭ�ϰ�, �ڷ� 10.0f��ŭ �̵��� ����
	toPlayer.Normalize();
	Vector2 desiredPosition = playerPos;
	//Vector2 desiredPosition = playerPos - (toPlayer * 30.0f); 

	m_Script->MoveTo(desiredPosition);
	// �� �Լ��� �ܼ��� �������� �����ϴ� ���Ҹ� ��.
	// ���� �̵��� UpdateMovement(context)���� RigidBody�� ���� �̷����.
}



void EnemyState::CheckTargetInAtkRange()
{
	if (!m_Script) return;

	const Vector2 playerPos = TargetPos();
	const Vector2 enemyPos = Pos();

	float distanceToPlayer = Vector2::Distance(enemyPos, playerPos);
	float attackRange = AttackRange();


	IsTargetInAtkRange() = (distanceToPlayer <= attackRange) ? true : false;

	IsTargetInMaxAtkRange() = (distanceToPlayer <= attackRange * 1.3f) ? true : false;

}

void EnemyState::CheckTargetInAtkAngle()
{
	if (!m_Script) return;

	const Vector2 enemyPos = Pos();
	const Vector2 targetPos = TargetPos();

	Vector2 toTarget = targetPos - enemyPos;


	if (toTarget.LengthSq() < MyMath::EPSILON)
	{
		IsTargetInAtkAngle() = true;
		return;
	}

	float currentEnemyAngleRad = RotationAngle() * (3.1415926535f / 180.0f);
	Vector2 enemyForward = Vector2(std::cosf(currentEnemyAngleRad), std::sinf(currentEnemyAngleRad));

	toTarget.Normalize();

	float dotProduct = Vector2::Dot(enemyForward, toTarget);

	float angleBetween = acosf(dotProduct) * (180.0f / 3.141592f); // ������ ���� ��ȯ

	float attackAngle = AtkAngle(); // Get the attack angle from EnemyBase

	// Check if the angle is within the attack angle (half of the total attack angle)
	if (angleBetween > (attackAngle / 2.0f))
	{
		IsTargetInAtkAngle() = false;
	}
	else
	{
		IsTargetInAtkAngle() = true;
	}
}
