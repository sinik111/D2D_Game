#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"

#include "EnemySlowTurn.h"

void EnemySlowTurn::Enter(FSMContext& context)
{	
	m_slowTurnTimer = 0.0f;

	const Vector2 currentPos = Pos();

	CheckTargetDistance();
	IsLockOnTarget() = true;
	
	const Vector2 targetPos = TargetPos();

	Vector2 toTarget = targetPos - currentPos;
		
	float targetAngleRadians = atan2f(toTarget.y, toTarget.x);
	
	float targetAngleDegree = targetAngleRadians * (180.0f / 3.1415926535f);
	
	if (targetAngleDegree < 0.0f) {
		targetAngleDegree += 360.0f;
	}

	m_Script->TargetSlowRotationAngle() = targetAngleDegree;
		
	if (IsPlayerDead(context) || IsTargetInChaseDist() == false)
	{
		IsLockOnTarget() = false;
		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN;
	}

	
	m_Script->StopMoving();
}


void EnemySlowTurn::Update(FSMContext& context)
{
	if (!m_Script) return;
	
	m_slowTurnTimer += MyTime::DeltaTime();
		
	float currentAngle = RotationAngle();

	// 단순 회전
	float rotationSpeed = m_Script->SlowRotationSpeed();
	float rotationThisFrame = rotationSpeed * MyTime::DeltaTime();

	// 목표 각도와 현재 각도 간의 차이 계산
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

	
	if (m_slowTurnTimer >= m_Script->RecorveryTime())
	{
		m_slowTurnTimer = 0.0f;
		context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;
	}
		
}

void EnemySlowTurn::Exit(FSMContext& context)
{
	
}
