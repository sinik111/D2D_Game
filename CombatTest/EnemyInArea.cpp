#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/Random.h"

#include <cmath>    
#include <random>   
#include <chrono>   
#include <thread>   

#include "EnemyInArea.h"


void EnemyInArea::Enter(FSMContext& context)
{
	m_isStateChanged = false;

	IsLockOnTarget() = false;
	IsTargetInChaseDist() = false;
	IsTargetInRoamDist() = false;
	IsUnderAttack() = false;
	IsFindPlayer() = false;		

	m_Script->SetAngleByDirection(Direction());
}

void EnemyInArea::Update(FSMContext& context)
{
	if (!m_Script) return;

	if (m_isStateChanged) return;
	
	CheckKnockdown(context);


	CheckTargetDistance();
	CheckTargetInAtkRange();
		
	if (!IsInCamera()) {
		
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::OUTOFAREA;
		return;
	}

	if (m_Script->IsUnderAttack())
	{
		
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::SLOWTURN;
		return;
	}

	if ((OriginPos() - Pos()).LengthSq() > MaxRoamDistance() * MaxRoamDistance())
	{
		
		m_isStateChanged = true;
		context.intParams[L"NextEnemyState"] = EnemyBase::RETURN;
		return;
	}

	PlayerSearch(context);
	

	if (IsFindPlayer())
	{		
		if (IsTargetInChaseDist())
		{			
			m_isStateChanged = true;
			context.intParams[L"NextEnemyState"] = EnemyBase::ENGAGE;
			return;
		}
	}
	SetRoam(context);
}

void EnemyInArea::Exit(FSMContext& context)
{
	m_isWaitingToRoam = false;
	m_roamWaitTimer = 0.0f;
	m_isRoaming = false;
}


void EnemyInArea::SetRoam(FSMContext& context)
{
	if (ToDoMove())
		return;

	//대기중이면 3초 동안 이동은 안함. 하지만 전체 업데이트함수에서 플레이어 찾기는 계속함.
	if (m_isWaitingToRoam)
	{
		m_roamWaitTimer += MyTime::DeltaTime();
		if (m_roamWaitTimer >= 3.0f) // 3초 대기
		{
			m_isWaitingToRoam = false;
			m_roamWaitTimer = 0.0f;
		}
		else
		{
			return;
		}
	}

	if (!ToDoMove() && !m_isRoaming) // ToDoMove()가 false이면 이동 중이 아니거나 목표에 도달한 상태
	{

		float maxRoamDistance = MaxRoamDistance();

		const Vector2 originPos = OriginPos();

		float randomDis = Random::Float(-maxRoamDistance, maxRoamDistance);
		//float randomDis = Random::Float(-50, 50);
		float sign = 1.0f;

		if (Random::Int(0, 1) == 0)
		{
			sign = -1.0f;
		}

		// 새로운 랜덤 목표 좌표 설정
		float randomX = originPos.x + randomDis;
		float randomY = originPos.y + randomDis * sign;

		m_Script->MoveTo(Vector2(randomX, randomY));

		//Debug::Log("이동목적지: ", MovingDestPos().x, "   /   ", MovingDestPos().y, "\n", randomDis);

		m_isRoaming = true;
	}

	if (!ToDoMove() && m_isRoaming) // 이동이 완료되어 ToDoMove가 false가 된 경우
	{
		m_Script->SetAngleByDirection(Direction());

		m_isWaitingToRoam = true; // 대기 상태 진입
		m_roamWaitTimer = 0.0f;   // 타이머 초기화
		m_isRoaming = false;
	}
}



void EnemyInArea::PlayerSearch(FSMContext& context)
{
	const Vector2 playerPos = TargetPos();
	const Vector2 enemyPos = Pos();

	IsFindPlayer() = SightCheck(playerPos, enemyPos);
	
}


bool EnemyInArea::SightCheck(const Vector2& playerPos, const Vector2& enemyPos)
{	
	Vector2 toPlayer = playerPos - enemyPos;
	float distanceToPlayerSq = toPlayer.LengthSq();

	float maxSightDistance = MaxSightDistance();
	float sightAngle = SightAngle();

	// 플레이어가 시야 거리 내에 있는지 1차 확인. 거리 밖이면 시야각 체크 불필요
	if (distanceToPlayerSq > maxSightDistance * maxSightDistance)
	{
		return false;
	}

	float currentEnemyAngleRad = RotationAngle() * (3.141592f / 180.0f); // 도로 라디안 변환
	
	Vector2 enemyForward = Vector2(cosf(currentEnemyAngleRad), sinf(currentEnemyAngleRad));

	toPlayer.Normalize();

	float targetAngleRadians = atan2f(toPlayer.y, toPlayer.x);
	float targetAngleDegrees = targetAngleRadians * (180.0f / 3.141592f);

	if (targetAngleDegrees < 0.0f) {
		targetAngleDegrees += 360.0f; // Normalize to 0-360 if necessary
	}
	
	float dotProduct = Vector2::Dot(enemyForward, toPlayer);

	float angleBetween = acosf(dotProduct) * (180.0f / 3.141592f); // 라디안을 도로 변환


	if (angleBetween > (sightAngle / 2.0f))
	{
		return false;
	}

	return true;
}





