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
//	//// 이동 명령이 없으면 아무것도 하지 않음
//	//if (!m_toDoMove)
//	//{
//	//	return;
//	//}
//
//	//// 1. 방향 및 거리 계산
//	//const Vector2 currentPosition = context.rigidBody2d->GetPosition();
//	//const float distance = Vector2::Distance(currentPosition, m_movingDestPos);
//
//	//// 2. 목표 지점 도착 처리
//	//if (distance < m_stoppingDistance)
//	//{
//	//	Stop();
//	//	return;
//	//}
//
//	//Vector2 direction = m_movingDestPos - currentPosition;
//	//direction.Normalize(); // 방향을 단위 벡터로 만듦
//
//	//// 3. 목표 방향으로 회전 처리 (부드러운 회전)
//	//// atan2를 사용하여 현재 위치에서 목표 지점으로 향하는 각도를 라디안 단위로 구함
//	//float targetAngle = atan2f(direction.y, direction.x);
//
//	//// 현재 각도와 목표 각도 사이를 부드럽게 보간(Lerp)
//	//float currentAngle = m_transform->GetRotation(); // 현재 각도를 가져오는 함수가 있다고 가정
//	//float newAngle = Math::LerpAngle(currentAngle, targetAngle, m_rotationSpeed * MyTime::DeltaTime());
//	//m_transform->SetRotation(newAngle); // 각도 설정
//
//	//// 4. 물리 가속 처리
//	//// 목표 속도 = 방향 * 최대 속도
//	//Vector2 desiredVelocity = direction * m_maxSpeed;
//	//Vector2 currentVelocity = m_rigidBody->GetVelocity();
//
//	//// 목표 속도와 현재 속도의 차이만큼 힘을 가해 가속
//	//Vector2 steeringForce = (desiredVelocity - currentVelocity) * m_acceleration;
//	//m_rigidBody->AddForce(steeringForce);
//}