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

	//ī�޶� ���� �ȿ� �ִ��� üũ�ϴ� ����. �´��� Ȯ�� �ʿ�.
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

	// ���� ���Ͱ� ������ 0 (�߸�)
	if (horizontalInput == 0.0f && verticalInput == 0.0f)
	{
		IsMove() = false;
		return;
	}

	IsMove() = true;

	// atan2�� y ����, x ����	
	float tAngle = std::atan2(horizontalInput, verticalInput) * 180.0f / 3.14159265f;

	SetAngle(tAngle);

}

void EnemyState::UpdateDirection()
{
	float degree = RotationAngle();

	// �ð�������� 8���� ����. 0���� UP, 180���� DOWN
	if (degree >= 337.5 || degree < 22.5)  Direction() = 8;			// Dir::UP;			// ��
	else if (degree >= 22.5 && degree < 67.5) Direction() = 9;		// Dir::RIGHT_UP;	// ��
	else if (degree >= 67.5 && degree < 112.5)  Direction() = 6;	// Dir::RIGHT;		// ��
	else if (degree >= 112.5 && degree < 157.5)   Direction() = 3;	// Dir::RIGHT_DOWN; // ��
	else if (degree >= 157.5 && degree < 202.5)  Direction() = 2;	// Dir::DOWN;		// ��
	else if (degree >= 202.5 && degree < 247.5)  Direction() = 1;	// Dir::LEFT_DOWN;	// ��
	else if (degree >= 247.5 && degree < 292.5) Direction() = 4;	// Dir::LEFT;		// ��
	else if (degree >= 292.5 && degree < 337.5) Direction() = 7;	// Dir::LEFT_UP;	// ��		
}

void EnemyState::SetAngle(float angle)
{
	// atan2�� y ����, x ����	
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

	// �̵� ����� ������ �ƹ��͵� ���� ����
	if (!ToDoMove())
	{
		return;
	}

	// 1. ���� �� �Ÿ� ���
	const Vector2 currentPosition = Pos();
	const float distance = Vector2::Distance(currentPosition, MovingDestPos());

	// 2. ��ǥ ���� ���� ó��
	if (distance < 0.001f)
	{
		StopMoving();
		return;
	}

	Vector2 direction = MovingDestPos() - currentPosition;
	direction.Normalize(); // ������ ���� ���ͷ� ����

	// 3. ��ǥ �������� ȸ�� ó�� (�ε巯�� ȸ��)
	// atan2�� ����Ͽ� ���� ��ġ���� ��ǥ �������� ���ϴ� ������ ���� ������ ����
	float targetAngle = atan2f(direction.y, direction.x);

	// ���� ������ ��ǥ ���� ���̸� �ε巴�� ����(Lerp)
	float currentAngle = RotationAngle(); // ���� ������ �������� �Լ��� �ִٰ� ����
	float newAngle = LerpAngle(currentAngle, targetAngle, rotationSpeed * MyTime::DeltaTime());
	RotationAngle() = newAngle; // ���� ����

	// 4. ���� ���� ó��
	// ��ǥ �ӵ� = ���� * �ִ� �ӵ�
	Vector2 desiredVelocity = direction * MoveSpeed();
	Vector2 currentVelocity = context.rigidBody2d->GetVelocity();

	// ��ǥ �ӵ��� ���� �ӵ��� ���̸�ŭ ���� ���� ����
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
	RigidBody()->SetVelocity(Vector2::Zero); // �ӵ��� ��� 0���� ����
	// m_transform->SetLocalPosition(m_movingDestPos); 		// ��ġ�� ��ǥ �������� ��Ȯ�� ���� ������ ������ ���� �ֽ��ϴ�.
}