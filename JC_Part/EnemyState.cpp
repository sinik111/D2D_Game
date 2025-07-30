#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/BoxCollider2D.h"

#include "../D2DEngineLib/FSMContext.h"

#include "EnemyState.h"


void EnemyState::Enter(FSMContext& context)
{
	//Init(context);
	SetDirection(context);
}

void EnemyState::Update(FSMContext& context)
{
	SetDirection(context);
}

void EnemyState::Exit(FSMContext& context)
{

}


void EnemyState::SetDirection(FSMContext& context)
{
	curDir = context.intParams[L"Direction"];	

	if(context.textRenderer)
		context.textRenderer->SetText(context.currentStateName + L" " + std::to_wstring(curDir));

	if (curDir != prevDir)
	{
		prevDir = curDir;

		context.animator->Play(animPath[curDir]);		
	}
}
//
//void EnemyState::UpdateMovement(FSMContext& context)
//{
//	//// �̵� ����� ������ �ƹ��͵� ���� ����
//	//if (!m_toDoMove)
//	//{
//	//	return;
//	//}
//
//	//// 1. ���� �� �Ÿ� ���
//	//const Vector2 currentPosition = context.rigidBody2d->GetPosition();
//	//const float distance = Vector2::Distance(currentPosition, m_movingDestPos);
//
//	//// 2. ��ǥ ���� ���� ó��
//	//if (distance < m_stoppingDistance)
//	//{
//	//	Stop();
//	//	return;
//	//}
//
//	//Vector2 direction = m_movingDestPos - currentPosition;
//	//direction.Normalize(); // ������ ���� ���ͷ� ����
//
//	//// 3. ��ǥ �������� ȸ�� ó�� (�ε巯�� ȸ��)
//	//// atan2�� ����Ͽ� ���� ��ġ���� ��ǥ �������� ���ϴ� ������ ���� ������ ����
//	//float targetAngle = atan2f(direction.y, direction.x);
//
//	//// ���� ������ ��ǥ ���� ���̸� �ε巴�� ����(Lerp)
//	//float currentAngle = m_transform->GetRotation(); // ���� ������ �������� �Լ��� �ִٰ� ����
//	//float newAngle = Math::LerpAngle(currentAngle, targetAngle, m_rotationSpeed * MyTime::DeltaTime());
//	//m_transform->SetRotation(newAngle); // ���� ����
//
//	//// 4. ���� ���� ó��
//	//// ��ǥ �ӵ� = ���� * �ִ� �ӵ�
//	//Vector2 desiredVelocity = direction * m_maxSpeed;
//	//Vector2 currentVelocity = m_rigidBody->GetVelocity();
//
//	//// ��ǥ �ӵ��� ���� �ӵ��� ���̸�ŭ ���� ���� ����
//	//Vector2 steeringForce = (desiredVelocity - currentVelocity) * m_acceleration;
//	//m_rigidBody->AddForce(steeringForce);
//}