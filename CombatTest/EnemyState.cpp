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


void EnemyState::RotateToTarget()
{
	if (!m_Script) return;

	const Vector2 enemyPos = Pos();
	const Vector2 targetPos = TargetPos(); // Get target's position

	Vector2 toTarget = targetPos - enemyPos;


	if (toTarget.LengthSq() < MyMath::EPSILON)
	{
		return;
	}

	float targetAngleRadians = atan2f(toTarget.y, toTarget.x);

	float targetAngleDegrees = targetAngleRadians * (180.0f / 3.141592f);
	if (targetAngleDegrees < 0.0f) {
		targetAngleDegrees += 360.0f; // Normalize to 0-360 if necessary
	}

	float currentAngleDegrees = RotationAngle(); // Get current rotation angle

	float rotationSpeed = 360.0f; // degrees per second
	float deltaRotation = rotationSpeed * MyTime::DeltaTime();

	float newAngle = LerpAngle(currentAngleDegrees, targetAngleDegrees, deltaRotation / 360.0f); // t should be 0-1
	// A simpler approach for "360 degrees per second" for delta rotation:
	// newAngle = LerpAngle(currentAngleDegrees, targetAngleDegrees, deltaRotation); // The 't' parameter here works differently based on your LerpAngle logic.
	// If your LerpAngle 't' is a direct angle to add/subtract, this is fine.
	// If 't' is a ratio (0.0 to 1.0), you need to calculate a ratio of how much of the remaining angle to turn.

	// Let's refine the LerpAngle usage for direct speed control:
	// Calculate the difference between current and target angle (shortest path)
	float angleDiff = fmod(targetAngleDegrees - currentAngleDegrees, 360.0f);
	if (angleDiff > 180.0f) angleDiff -= 360.0f;
	if (angleDiff < -180.0f) angleDiff += 360.0f;

	// Determine how much to rotate this frame
	float rotationThisFrame = rotationSpeed * MyTime::DeltaTime();

	if (std::abs(angleDiff) <= rotationThisFrame) {
		// If the remaining angle is less than or equal to what we can rotate this frame,
		// just snap to the target angle to avoid overshooting.
		RotationAngle() = targetAngleDegrees;
	}
	else {
		// Rotate by `rotationThisFrame` in the correct direction
		if (angleDiff > 0) {
			RotationAngle() += rotationThisFrame;
		}
		else {
			RotationAngle() -= rotationThisFrame;
		}
		// Ensure the angle stays within 0-360 range after rotation
		float& currentRotAngle = RotationAngle(); // Get ref to modify directly
		if (currentRotAngle < 0.0f) currentRotAngle += 360.0f;
		if (currentRotAngle >= 360.0f) currentRotAngle -= 360.0f;
	}

	// After changing RotationAngle(), it's a good practice to update the Direction if needed

}

void EnemyState::CheckTargetInAtkAngle()
{
	if (!m_Script) return;

	const Vector2 enemyPos = Pos();
	const Vector2 targetPos = TargetPos(); // Get target's position

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



	//void EnemyState::SetEnemyDirectionByInput(FSMContext & context)
	//{
	//	float horizontalInput = context.floatParams[L"HorizontalInput"];
	//	float verticalInput = context.floatParams[L"VerticalInput"];

	//	// 방향 벡터가 없으면 0 (중립)
	//	if (horizontalInput == 0.0f && verticalInput == 0.0f)
	//	{
	//		return;
	//	}

	//	// atan2는 y 먼저, x 나중	
	//	float tAngle = std::atan2(horizontalInput, verticalInput) * 180.0f / 3.14159265f;

	//	m_Script->SetRotationAngle(tAngle);
	//}