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

	const Vector2 playerPos = TargetPos(); // 플레이어 위치
	const Vector2 originPos = OriginPos(); // 적의 스폰 지점 (원점)

	float distancePlayerToOrigin = Vector2::Distance(playerPos, originPos); // 플레이어와 원점 사이 거리

	float maxChaseDistance = MaxChaseDistance(); // 추격 최대 거리
	float maxRoamDistance = MaxRoamDistance();   // 이동 최대 거리 (문제에서 MaxMoveDistance로 언급)


	IsTargetInRoamDist() = (distancePlayerToOrigin <= maxRoamDistance) ? true : false;

	IsTargetInChaseDist() = (distancePlayerToOrigin <= maxChaseDistance) ? true : false;

}



void EnemyState::MoveToPlayer()
{
	if (!m_Script) return;

	const Vector2 playerPos = TargetPos();
	const Vector2 enemyPos = Pos();

	// 플레이어와 자신의 위치의 직선방향 벡터
	Vector2 toPlayer = playerPos - enemyPos;
	float distanceToPlayer = toPlayer.Length();

	// 목표 지점 설정: 플레이어로부터 10.0f만큼 떨어진 위치
	// 방향 벡터를 정규화하고, 뒤로 10.0f만큼 이동한 지점
	toPlayer.Normalize();
	Vector2 desiredPosition = playerPos;
	//Vector2 desiredPosition = playerPos - (toPlayer * 30.0f); 

	m_Script->MoveTo(desiredPosition);
	// 이 함수는 단순히 목적지를 설정하는 역할만 함.
	// 실제 이동은 UpdateMovement(context)에서 RigidBody를 통해 이루어짐.
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
