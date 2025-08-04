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



	//void EnemyState::SetEnemyDirectionByInput(FSMContext & context)
	//{
	//	float horizontalInput = context.floatParams[L"HorizontalInput"];
	//	float verticalInput = context.floatParams[L"VerticalInput"];

	//	// ���� ���Ͱ� ������ 0 (�߸�)
	//	if (horizontalInput == 0.0f && verticalInput == 0.0f)
	//	{
	//		return;
	//	}

	//	// atan2�� y ����, x ����	
	//	float tAngle = std::atan2(horizontalInput, verticalInput) * 180.0f / 3.14159265f;

	//	m_Script->SetRotationAngle(tAngle);
	//}