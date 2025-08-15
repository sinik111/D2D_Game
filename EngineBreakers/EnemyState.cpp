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
	if (!m_Script)	{ return; }		

	const Vector2 playerPos = TargetPos(); 
	const Vector2 originPos = OriginPos(); 

	float distancePlayerToOriginSq = (playerPos - originPos).LengthSq();

	float maxChaseDistanceSq = MaxChaseDistance() * MaxChaseDistance();
	float maxRoamDistanceSq = MaxRoamDistance() * MaxRoamDistance();  


	IsTargetInRoamDist() = (distancePlayerToOriginSq <= maxRoamDistanceSq) ? true : false;

	IsTargetInChaseDist() = (distancePlayerToOriginSq <= maxChaseDistanceSq) ? true : false;

	//if (IsTargetInRoamDist())
	//	m_Script->m_distanceText = L"Distance: In Roam";
	//else if (IsTargetInChaseDist())
	//	m_Script->m_distanceText = L"Distance: In Chase";
	//else
	//	m_Script->m_distanceText = L"Distance Out";
}



void EnemyState::MoveToPlayer()
{
	if (!m_Script) return;

	const Vector2 playerPos = TargetPos();
	const Vector2 enemyPos = Pos();

	
	Vector2 toPlayer = playerPos - enemyPos;	

	if (toPlayer.LengthSq() <= MyMath::EPSILON)
		return;

	toPlayer.Normalize();

	Vector2 desiredPosition;

	if (m_Script->m_enemyType == EnemyBase::BOSS)
	{
		desiredPosition = playerPos - (toPlayer * 130.0f);
	}else
	{
		desiredPosition = playerPos - (toPlayer * 70.0f);
	}

	m_Script->MoveTo(desiredPosition);
	
	// 이 함수는 단순히 목적지를 설정하는 역할만 함.
	// 실제 이동은 UpdateMovement(context)에서 RigidBody를 통해 이루어짐.
}



void EnemyState::CheckTargetInAtkRange()
{
	if (!m_Script) return;

	const Vector2 playerPos = TargetPos();
	const Vector2 enemyPos = Pos();

	float distanceToPlayerSq = (enemyPos - playerPos).LengthSq();
	
	float attackRangeSq = AttackRange() * AttackRange();


	IsTargetInAtkRange() = (distanceToPlayerSq <= attackRangeSq) ? true : false;

	IsTargetInMaxAtkRange() = (distanceToPlayerSq <= attackRangeSq + AttackRange() * 0.25f ) ? true : false;

	//if (IsTargetInAtkRange())
	//	m_Script->m_attackRangeText = L"Atk Range: In";
	//else if (IsTargetInMaxAtkRange())
	//	m_Script->m_attackRangeText = L"Atk Range: In Max";
	//else
	//	m_Script->m_attackRangeText = L"Atk Range: OUT";

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

	float angleBetween = acosf(dotProduct) * (180.0f / 3.141592f); // 라디안을 도로 변환

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

void EnemyState::CheckKnockdown(FSMContext& context)
{
	if (m_Script->IsKnockdown())
	{
		m_Script->m_knockdownAccumulated = 0;

		m_isStateChanged = true;		
		context.intParams[L"NextEnemyState"] = EnemyBase::KNOCKDOWN;		
		return;
	}
}
