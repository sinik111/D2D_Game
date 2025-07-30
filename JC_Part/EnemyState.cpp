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

void EnemyState::CheckCameraArea() {

	//카메라 범위 안에 있는지 체크하는 조건. 맞는지 확인 필요.
	Vector2 camPos = CameraPos();
	Vector2 myPos = Pos();

	if (myPos.x < camPos.x + 960 &&
		myPos.x > camPos.x - 960 &&
		myPos.y < camPos.y + 540 &&
		myPos.y > camPos.y - 540)
	{
		IsInCamera() = true;
		return;
	}

	IsInCamera() = false;
}



void EnemyState::SetEnemyDirectionByInput(FSMContext& context)
{
	float horizontalInput = context.floatParams[L"HorizontalInput"];
	float verticalInput = context.floatParams[L"VerticalInput"];

	// 방향 벡터가 없으면 0 (중립)
	if (horizontalInput == 0.0f && verticalInput == 0.0f)
	{
		IsMove() = false;
		return;
	}

	IsMove() = true;

	// atan2는 y 먼저, x 나중	
	float tAngle = std::atan2(horizontalInput, verticalInput) * 180.0f / 3.14159265f;

	SetAngle(tAngle);

}

void EnemyState::UpdateDirection()
{
	float degree = RotationAngle();

	// 시계방향으로 8방향 판정. 0도가 UP, 180도가 DOWN
	if (degree >= 337.5 || degree < 22.5)  Direction() = 8;			// Dir::UP;			// ↑
	else if (degree >= 22.5 && degree < 67.5) Direction() = 9;		// Dir::RIGHT_UP;	// ↗
	else if (degree >= 67.5 && degree < 112.5)  Direction() = 6;	// Dir::RIGHT;		// →
	else if (degree >= 112.5 && degree < 157.5)   Direction() = 3;	// Dir::RIGHT_DOWN; // ↘
	else if (degree >= 157.5 && degree < 202.5)  Direction() = 2;	// Dir::DOWN;		// ↓
	else if (degree >= 202.5 && degree < 247.5)  Direction() = 1;	// Dir::LEFT_DOWN;	// ↙
	else if (degree >= 247.5 && degree < 292.5) Direction() = 4;	// Dir::LEFT;		// ←
	else if (degree >= 292.5 && degree < 337.5) Direction() = 7;	// Dir::LEFT_UP;	// ↖		
}

void EnemyState::SetAngle(float angle)
{
	// atan2는 y 먼저, x 나중	
	RotationAngle() = angle;

	float& degree = RotationAngle();

	if (RotationAngle() < 0.0f) RotationAngle() += 360.0f;

	UpdateDirection();
}

void EnemyState::SetDirection(int n)
{
	Direction() = n;

	if (Direction() == 8)		RotationAngle() = 0.0f;
	else if (Direction() == 9)	RotationAngle() = 45.0f;
	else if (Direction() == 6)	RotationAngle() = 90.0f;
	else if (Direction() == 3)	RotationAngle() = 135.0f;
	else if (Direction() == 2)	RotationAngle() = 180.0f;
	else if (Direction() == 1)	RotationAngle() = 225.0f;
	else if (Direction() == 4)	RotationAngle() = 270.0f;
	else if (Direction() == 7)	RotationAngle() = 315.0f;
}




void EnemyState::UpdateMovement(FSMContext& context)
{
	float rotationSpeed = 2.0f;
	float acceleration = 10.0f;

	// 이동 명령이 없으면 아무것도 하지 않음
	if (!ToDoMove())
	{
		return;
	}

	// 1. 방향 및 거리 계산
	const Vector2 currentPosition = Pos();
	const float distance = Vector2::Distance(currentPosition, MovingDestPos());

	// 2. 목표 지점 도착 처리
	if (distance < 0.001f)
	{
		StopMoving();
		return;
	}

	Vector2 direction = MovingDestPos() - currentPosition;
	direction.Normalize(); // 방향을 단위 벡터로 만듦

	// 3. 목표 방향으로 회전 처리 (부드러운 회전)
	// atan2를 사용하여 현재 위치에서 목표 지점으로 향하는 각도를 라디안 단위로 구함
	float targetAngle = atan2f(direction.y, direction.x);

	// 현재 각도와 목표 각도 사이를 부드럽게 보간(Lerp)
	float currentAngle = RotationAngle(); // 현재 각도를 가져오는 함수가 있다고 가정
	float newAngle = LerpAngle(currentAngle, targetAngle, rotationSpeed * MyTime::DeltaTime());
	RotationAngle() = newAngle; // 각도 설정

	// 4. 물리 가속 처리
	// 목표 속도 = 방향 * 최대 속도
	Vector2 desiredVelocity = direction * MoveSpeed();
	Vector2 currentVelocity = context.rigidBody2d->GetVelocity();

	// 목표 속도와 현재 속도의 차이만큼 힘을 가해 가속
	Vector2 steeringForce = (desiredVelocity - currentVelocity) * acceleration;
	context.rigidBody2d->AddForce(steeringForce);
}

RigidBody2D* EnemyState::RigidBody()
{ 
	return m_Script->RigidBody();
}

void EnemyState::StopMoving()
{
	ToDoMove() = false;
	RigidBody()->SetVelocity(Vector2::Zero); // 속도를 즉시 0으로 만듦
	// m_transform->SetLocalPosition(m_movingDestPos); 		// 위치를 목표 지점으로 정확히 맞춰 오차를 보정할 수도 있습니다.
}