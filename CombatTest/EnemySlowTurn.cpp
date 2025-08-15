#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemySlowTurn.h"

void EnemySlowTurn::Enter(FSMContext& context)
{
	m_isStateChanged = false;

	m_isTurnEnd = false;

	m_slowTurnTimer = 0.0f;

	const Vector2 currentPos = Pos();

	IsLockOnTarget() = true;

	const Vector2 targetPos = TargetPos();

	Vector2 toTarget = targetPos - currentPos;

	float targetAngleRadians = atan2f(toTarget.y, toTarget.x);

	float targetAngleDegree = targetAngleRadians * (180.0f / 3.1415926535f);

	targetAngleDegree = fmodf(targetAngleDegree, 360.0f); 

	if (targetAngleDegree < 0.0f) {
		targetAngleDegree += 360.0f; 
	}

	m_Script->TargetSlowRotationAngle() = targetAngleDegree;

	if (IsPlayerDead(context) || IsTargetInChaseDist() == false)
	{
		IsLockOnTarget() = false;
		context.intParams[L"NextEnemyState"] = EnemyBase::EVADEANDRUN;
	}


	m_Script->StopMoving();
}


void EnemySlowTurn::Update(FSMContext& context)
{
	if (!m_Script) return;

	if (m_isStateChanged) return;

	CheckKnockdown(context);

	CheckTargetDistance();
	CheckTargetInAtkRange();

	m_slowTurnTimer += MyTime::DeltaTime();

	float currentAngle = RotationAngle();

	// �ܼ� ȸ��
	float rotationSpeed = m_Script->SlowRotationSpeed();
	float rotationThisFrame = rotationSpeed * MyTime::DeltaTime();

	// ��ǥ ������ ���� ���� ���� ���� ���
	float angleDiff = m_Script->TargetSlowRotationAngle() - currentAngle;

	angleDiff = fmod(angleDiff, 360.0f);

	if (angleDiff > 180.0f) angleDiff -= 360.0f;
	if (angleDiff < -180.0f) angleDiff += 360.0f;

	bool isRotationFinished = std::abs(angleDiff) <= MyMath::EPSILON;


	if (!isRotationFinished)
	{
		if (std::abs(angleDiff) <= rotationThisFrame)
		{
			m_Script->RotationAngle() = m_Script->TargetSlowRotationAngle();
			m_isTurnEnd = true;
		}
		else
		{
			if (angleDiff > 0)
			{
				m_Script->RotationAngle() = currentAngle + rotationThisFrame;
			}
			else
			{
				m_Script->RotationAngle() = currentAngle - rotationThisFrame;
			}
		}
	}

	if (m_isTurnEnd || std::abs(angleDiff) <= MyMath::EPSILON)
	{
		m_slowTurnTimer = 0.0f;
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;
		return;
	}

	//if (m_slowTurnTimer >= m_Script->RecorveryTime())
	//{
	//	m_slowTurnTimer = 0.0f;
	//	m_isStateChanged = true;
	//	context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;
	//	return;
	//}

}

void EnemySlowTurn::Exit(FSMContext& context)
{

}
