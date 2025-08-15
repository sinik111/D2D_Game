#include "../D2DEngineLib/framework.h"
#include "EnemyBase.h"

#include <limits>
#include <iostream>

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/FSMContext.h"

#include "EnemyBaseMove.h"
#include "EnemyBaseIdle.h"

#include "EnemyOutOfArea.h"
#include "EnemyInArea.h"
#include "EnemyEngage.h"
#include "EnemyOnAttack.h"
#include "EnemyReturn.h"
#include "EnemyOnEvade.h"

#include "TempEnemyAttack.h"



void EnemyBase::Initialize()
{
	//m_playerInput = GetGameObject()->GetComponent<PlayerInput>();
	m_animator = GetGameObject()->GetComponent<Animator>();
	m_rigidBody = GetGameObject()->GetComponent<RigidBody2D>();
	m_textRenderer = GetGameObject()->GetComponent<TextRenderer>();

	m_textRenderer->SetText(L"IdleState : 2");
	//m_textRenderer->GetTransform()->SetLocalPosition(m_rigidBody->GetPosition());
	m_textRenderer->SetFontSize(15.f);
	m_textRenderer->SetHorizontalAlignment(HorizontalAlignment::Center);
	m_textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
	m_textRenderer->SetRectSize({ 120.0f, 40.0f });
	m_textRenderer->SetSpaceType(SpaceType::World);
	m_textRenderer->SetSortOrder(10);
	m_textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));

	//콜라이더
	m_collider = GetGameObject()->GetComponent<BoxCollider2D>();

	vSetting();
}

void EnemyBase::Start()
{
	//m_playerInput->RegisterDirectionAction(DirectionInputType::Arrow, this, &EnemyBase::ArrowInput);

	m_animator->AddSpriteSheet(L"EnemyBase_sprites.json");

	m_animator->AddAnimationClip(L"EnemyBase_Move_UP_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_DOWN_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_LEFT_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_RIGHT_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_RIGHT_UP_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_RIGHT_DOWN_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_LEFT_UP_anim.json");
	m_animator->AddAnimationClip(L"EnemyBase_Move_LEFT_DOWN_anim.json");

	animPath[0] = L"EnemyBase_Move_UP";
	animPath[1] = L"EnemyBase_Move_LEFT_DOWN";
	animPath[2] = L"EnemyBase_Move_DOWN";
	animPath[3] = L"EnemyBase_Move_RIGHT_DOWN";
	animPath[4] = L"EnemyBase_Move_LEFT";
	animPath[5] = L"EnemyBase_Move_UP";
	animPath[6] = L"EnemyBase_Move_RIGHT";
	animPath[7] = L"EnemyBase_Move_LEFT_UP";
	animPath[8] = L"EnemyBase_Move_UP";
	animPath[9] = L"EnemyBase_Move_RIGHT_UP";

	m_context.gameObject = GetGameObject();
	m_context.animator = m_animator;
	m_context.transform = GetTransform();
	m_context.bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
	m_context.rigidBody2d = m_rigidBody;
	m_context.floatParams[L"HorizontalInput"] = 0.0f;
	m_context.floatParams[L"VerticalInput"] = 0.0f;


	m_context.intParams[L"PrevEnemyState"] = m_prevEnemyState;
	m_context.boolParams[L"IsPlayerDead"] = m_isPlayerDead;
	m_context.boolParams[L"IsFindPlayer"] = m_isFindPlayer;


	m_context.textRenderer = m_textRenderer;

	m_textRenderer->SetText(L"IdleState : ");

	m_rigidBody->SetGravityScale(0.0f);

	m_context.gameObject->GetTransform()->SetLocalScale({ 0.6f, 0.6f });

	Vector2 tPosition = {
		m_context.gameObject->GetTransform()->GetLocalPosition().x,
		m_context.gameObject->GetTransform()->GetLocalPosition().y };

	PositionInit(tPosition.x, tPosition.y, -90.0f);

	m_maxRoamDistance = 800.0f;
	m_maxChaseDistance = 900.0f;

	m_fsm.AddState<EnemyOutOfArea>(L"OutOfArea", false, this);
	m_fsm.AddState<EnemyInArea>(L"InArea", false, this);
	m_fsm.AddState<EnemyEngage>(L"Engage", false, this);
	m_fsm.AddState<EnemyOnAttack>(L"OnAttack", false, this);
	m_fsm.AddState<EnemyReturn>(L"Return", false, this);
	m_fsm.AddState<EnemyOnEvade>(L"OnEvade", false, this);

	m_fsm.SetState(L"OutOfArea", m_context);

	m_context.intParams[L"PrevEnemyState"] = NOSTATE;
	m_context.intParams[L"NextEnemyState"] = NOSTATE;
	m_context.intParams[L"CurrEnemyState"] = OUTOFAREA;


	//콜라이더 세팅
	m_collider->SetSize({ 40.0f, 80.0f });
	m_collider->SetOffset({ 0.0f, 0.0f });
	m_collider->SetTrigger(false);
	m_collider->SetLayer(CollisionLayer::EnemyHitBox);


}
void EnemyBase::FixedUpdate()
{

	RigidBodyUpdate();
}

void EnemyBase::Update()
{

	CheckAndTransitState();

	CheckCameraArea();

	AttackCoolCheck();

	UnderAttackManage();


	m_fsm.Update(m_context);

	if (IsKnockBack())
	{
		UpdateKnockBack();
	}

	UpdateDirection();
	UpdateMovement();

}

void EnemyBase::OnDestroy()
{

}


void EnemyBase::ArrowInput(Vector2 input)
{
	m_context.floatParams[L"HorizontalInput"] = input.x;
	m_context.floatParams[L"VerticalInput"] = input.y;
}

void EnemyBase::CheckAndTransitState()
{
	m_nextEnemyState = m_context.intParams[L"NextEnemyState"];

	if (m_nextEnemyState == NOSTATE)
		return;

	m_prevEnemyState = m_currEnemyState;
	m_context.intParams[L"PrevEnemyState"] = m_prevEnemyState;


	m_currEnemyState = m_nextEnemyState;
	m_context.intParams[L"CurrEnemyState"] = m_currEnemyState;


	switch (m_currEnemyState)
	{
	case OUTOFAREA:
		//std::cout << m_currEnemyState << std::endl;
		m_context.nextStateName = L"OutOfArea";
		break;

	case INAREA:
		//std::cout << m_currEnemyState << std::endl;
		m_context.nextStateName = L"InArea";
		break;

	case ENGAGE:
		//std::cout << m_currEnemyState << std::endl;
		m_context.nextStateName = L"Engage";
		break;

	case ONATTACK:
		//std::cout << m_currEnemyState << std::endl;
		m_context.nextStateName = L"OnAttack";
		break;

	case RETURN:
		//std::cout << m_currEnemyState << std::endl;
		m_context.nextStateName = L"Return";
		break;

	case ONEVADE:
		//std::cout << m_currEnemyState << std::endl;
		m_context.nextStateName = L"OnEvade";
		break;
	}

	m_context.shouldChangeState = true;

	m_nextEnemyState = NOSTATE;
	m_context.intParams[L"NextEnemyState"] = m_nextEnemyState; //NOSTATE

}


void EnemyBase::SetTargetPlayerAndCamera(GameObject* player, GameObject* camera)
{
	m_player = player;
	m_camera = camera;
}

void EnemyBase::PositionInit(float x, float y, float angle)
{
	m_rigidBody->SetPosition({ x, y });
	m_originPos = m_rigidBody->GetPosition();
	SetRotationAngle(angle);
}

void EnemyBase::CheckCameraArea() {

	//카메라 범위 안에 있는지 체크하는 조건. 맞는지 확인 필요.
	Vector2 camPos = CameraPos();
	Vector2 myPos = Pos();
	Vector2 offset = Vector2(0.0f, 250.0f);

	if (myPos.x < camPos.x + 800 &&
		myPos.x > camPos.x - 800 &&
		myPos.y < camPos.y + 400 &&
		myPos.y > camPos.y - 400 - offset.y)
	{
		IsInCamera() = true;
		return;
	}

	IsInCamera() = false;
}



////---------------------------------------------------------------------------------------------------------------------------
//이동 관련//--------------------------------------------------------------------------------------------------------------------

void EnemyBase::UpdateMovement()
{
	if (!ToDoMove()) return;

	const Vector2 currentPosition = Pos();
	const float distanceSq = (DepaturePos() - currentPosition).LengthSq();

	if (distanceSq >= DistanceToMoveSq())
	{
		StopMoving();
		return;
	}
}


void EnemyBase::RigidBodyUpdate()
{
	Vector2 desiredVelocity = m_aheadDirection * MoveSpeed();
	Vector2 currentVelocity = m_rigidBody->GetVelocity();

	Vector2 steeringForce = (desiredVelocity - currentVelocity) * m_acceleration;

	float maxSteeringForce = 1500.0f;
	if (steeringForce.LengthSq() > maxSteeringForce * maxSteeringForce) {
		steeringForce = steeringForce.Normalized() * maxSteeringForce;
	}

	m_rigidBody->AddForce(steeringForce);
}


void EnemyBase::MoveTo(const Vector2& destination)
{
	const Vector2 currentPos = Pos();

	DepaturePos() = currentPos;

	Vector2 direction = destination - currentPos;

	DistanceToMoveSq() = direction.LengthSq();
	
	MovingDestPos() = destination;

	ToDoMove() = true;
	
	direction.Normalize();

	m_aheadDirection = direction;

	SetAngleByAheadDirection(m_aheadDirection);

	m_acceleration = 200.0f;
}


void EnemyBase::Evade()
{

	const Vector2 currentPos = Pos();
	DepaturePos() = currentPos;

	const Vector2 targetPos = TargetPos();

	Vector2 evadeDirection = currentPos - targetPos;
		
	evadeDirection.Normalize();

	const float distance = m_evadeDistance;

	MovingDestPos() = currentPos + (evadeDirection * distance);

	DistanceToMoveSq() = (MovingDestPos() - DepaturePos()).LengthSq();

	ToDoMove() = true;

	m_aheadDirection = evadeDirection;

	SetAngleByAheadDirection(m_aheadDirection);
	m_acceleration = 250.0f;
}


void EnemyBase::SetRotationAngle(float angle)
{
	m_rotationAngle = angle;

	if (m_rotationAngle < 0.0f) m_rotationAngle += 360.0f;
}


void EnemyBase::AheadToTarget()
{
	Vector2 direction = TargetPos() - Pos();

	direction.Normalize();

	m_aheadDirection = direction;

	SetAngleByAheadDirection(m_aheadDirection);
}


void EnemyBase::StopMoving()
{
	ToDoMove() = false;
	MovingDestPos() = Pos();
	DistanceToMoveSq() = 0.0f;
	m_rigidBody->SetVelocity(Vector2::Zero);
	Acceleration() = 0.0f;
}


void EnemyBase::SetAnimDirection()
{
	CurrDir() = Direction();

	if (m_textRenderer)
		m_textRenderer->SetText(m_context.currentStateName + L" " + std::to_wstring(CurrDir()));

	if (CurrDir() != PrevDir())
	{
		PrevDir() = CurrDir();

		m_animator->Play(animPath[CurrDir()]);
	}
}

void EnemyBase::UpdateDirection()
{
	if (m_rotationAngle >= 337.5f || m_rotationAngle < 22.5f)       m_direction = 6;  // Dir::RIGHT; // → (0도 중심)
	else if (m_rotationAngle >= 22.5f && m_rotationAngle < 67.5f)    m_direction = 9;  // Dir::RIGHT_UP; // ↗ (45도 중심)
	else if (m_rotationAngle >= 67.5f && m_rotationAngle < 112.5f)   m_direction = 8;  // Dir::UP; // ↑ (90도 중심)
	else if (m_rotationAngle >= 112.5f && m_rotationAngle < 157.5f)  m_direction = 7;  // Dir::LEFT_UP; // ↖ (135도 중심)
	else if (m_rotationAngle >= 157.5f && m_rotationAngle < 202.5f)  m_direction = 4;  // Dir::LEFT; // ← (180도 중심)
	else if (m_rotationAngle >= 202.5f && m_rotationAngle < 247.5f)  m_direction = 1;  // Dir::LEFT_DOWN; // ↙ (225도 중심)
	else if (m_rotationAngle >= 247.5f && m_rotationAngle < 292.5f)  m_direction = 2;  // Dir::DOWN; // ↓ (270도 중심)
	else if (m_rotationAngle >= 292.5f && m_rotationAngle < 337.5f)  m_direction = 3;  // Dir::RIGHT_DOWN; // ↘ (315도 중심)

	SetAnimDirection();
}


void EnemyBase::SetAngleByDirection(int n)
{
	m_direction = n;

	if (m_direction == 6)		m_rotationAngle = 0.0f;
	else if (m_direction == 9)	m_rotationAngle = 45.0f;
	else if (m_direction == 8)	m_rotationAngle = 90.0f;
	else if (m_direction == 7)	m_rotationAngle = 135.0f;
	else if (m_direction == 4)	m_rotationAngle = 180.0f;
	else if (m_direction == 1)	m_rotationAngle = 225.0f;
	else if (m_direction == 2)	m_rotationAngle = 270.0f;
	else if (m_direction == 3)	m_rotationAngle = 315.0f;
}

void EnemyBase::SetAngleByAheadDirection(const Vector2& adir)
{
	float targetAngleRadians = atan2f(adir.y, adir.x);

	float targetAngleDegrees = targetAngleRadians * (180.0f / 3.141592f);

	SetRotationAngle(targetAngleDegrees);
}


///////////-------------------------------------------------------------------------------------------------------------------------------
//공격관련//-------------------------------------------------------------------------------------------------------------------------------

void EnemyBase::AttackCoolCheck()
{
	if (IsAttackReady())
		return;

	AttackTimer() += MyTime::DeltaTime();

	if (AttackInterval() <= AttackTimer())
	{
		IsAttackReady() = true;
		AttackTimer() = 0.0f;
	}
}

void EnemyBase::FakeAttack()
{
	if (!m_isAttackReady)
		return;

	//Debug::Log("어택이다");
	//m_atkCollider = CreateGameObject(m_tempLabel);
	//m_atkCollider->AddComponent<TextRenderer>();
	//m_atkCollider->AddComponent<RigidBody2D>();
	//m_atkCollider->AddComponent<BoxCollider2D>();
	//m_atkCollider->AddComponent<TempEnemyAttackCollider>(this);



	m_isAttackReady = false;
}


///////////-------------------------------------------------------------------------------------------------------------------------------
//피격관련//-------------------------------------------------------------------------------------------------------------------------------

void EnemyBase::EnemyUnderAttack(const Collision& collision)
{
	Debug::Log("유아언더어택!");
	if (collision.otherGameObject->GetName() == L"DummyPlayerAttack")
	{
		m_isUnderAttack = true;
		m_underAttackTimer = 0.0f;
	}
}


void EnemyBase::UnderAttackManage()
{
	if (!m_isUnderAttack)
		return;

	m_underAttackTimer += MyTime::DeltaTime();

	if (m_underAttackTimer >= m_underAttackValidTime)
	{
		m_underAttackTimer = 0.0f;
		m_isUnderAttack = false;
	}
}

void EnemyBase::KnockBack(const float& dist, const float& speed)
{
	Debug::Log("넉빽!");

	if (!m_rigidBody) return;

	if (!IsKnockBack())
	{
		StopMoving();
	}

	IsKnockBack() = true;
	m_knockbackCurrentTimer = 0.0f;


	const Vector2 enemyPos = Pos();
	const Vector2 playerPos = TargetPos();
	Vector2 knockBackDirection = enemyPos - playerPos;

	knockBackDirection.Normalize();
	m_knockbackDirection = knockBackDirection;

	AheadDirection() = knockBackDirection * -1.0f;
	SetAngleByAheadDirection(AheadDirection());

	m_knockbackSpeed = dist * speed;
}


void EnemyBase::UpdateKnockBack()
{
	if (!m_rigidBody) return;

	m_knockbackCurrentTimer += MyTime::DeltaTime();

	// 넉백 종료 조건
	if (m_knockbackCurrentTimer >= m_knockbackDuration)
	{
		StopMoving(); // 최종적으로 속도 0으로 정지
		IsKnockBack() = false; // 넉백 상태 비활성화
		m_knockbackCurrentTimer = 0.0f;
		m_knockbackSpeed = 0.0f;
		m_rigidBody->SetVelocity(Vector2::Zero); // 확실히 멈춤
		return;
	}

	float t = m_knockbackCurrentTimer / m_knockbackDuration;

	// 넉백 속도 계산 (시작 속도에서 0으로 지수함수 형태로 감소)
	// Lerp를 사용하여 부드럽게 감속시킵니다.
	// 1.0 - t는 t가 0일 때 1.0, t가 1일 때 0이 되어, 시작 속도에서 0으로 선형적으로 감소합니다.
	// powf를 사용하여 지수적 감소 효과를 더할 수 있습니다. (예: powf(1.0f - t, 2.0f) 등)
	// 여기서는 선형 감속을 기본으로 하고, 더 강한 감속을 원하면 powf를 추가하세요.
	float currentKnockBackSpeed = m_knockbackSpeed * ((1.0f - t) * (1.0f - t));

	// 지수적 감소를 원한다면 (예: t가 0일 때 1, t가 1일 때 0에 수렴하도록)
	// float decayFactor = 1.0f - t; // 선형 감소
	// float decayFactor = powf(1.0f - t, 2.0f); // 이차 함수 감소 (더 빠르게 줄어듬)
	// float decayFactor = expf(-t * 5.0f); // 실제 지수 함수 감소 (5.0f는 감쇠 속도 조절, 클수록 빨리 감소)
	// currentKnockBackSpeed = m_knockbackSpeed * decayFactor;

	m_rigidBody->SetVelocity(m_knockbackDirection * currentKnockBackSpeed);

}






RigidBody2D* EnemyBase::RigidBody()
{
	return m_rigidBody;
}


const Vector2& EnemyBase::Pos()
{
	return m_rigidBody->GetTransform()->GetLocalPosition();
}




void EnemyBase::OnCollisionEnter(const Collision& collision)
{
	Debug::Log(L"EnemyBase OnCollisionEnter");
}

void EnemyBase::OnCollisionStay(const Collision& collision)
{

}

void EnemyBase::OnCollisionExit(const Collision& collision)
{

}

void EnemyBase::OnTriggerEnter(const Collision& collision)
{
	Debug::Log(L"EnemyBase OnTriggerEnter");
	EnemyUnderAttack(collision);
}

void EnemyBase::OnTriggerExit(const Collision& collision)
{
	Debug::Log(L"EnemyBase OnTriggerExit");
}


void EnemyBase::vSetting()
{
	m_isDead = false;

	m_isInCamera = false;

	m_rotationAngle = 0.0f;
	//m_direction;

	m_isPlayerDead = false;

	//m_originPos;
	//m_originAngle;

	m_toDoMove = false;
	//m_movingDestPos;
	//m_moveDirection;

	m_rotationSpeed = 300.0f;
	m_acceleration = 0.0f;

	m_moveSpeed = 200.0f;
	m_maxSightDistance = 700.0f;
	m_sightAngle = 90.0f;
	m_maxRoamDistance = 1000.0f;
	m_maxChaseDistance = 1200.0f;
	m_AttackRange = 200.0f;

	m_isLockOnTarget = false;
	m_isTargetInChaseDist = false;
	m_isTargetInRoamDist = false;
	m_isTargetInAtkRange = false;

	m_atkAngle = 20.0f;
	m_isTargetInAtkAngle = false;

	m_attackInterval = 3.0f;
	m_attackTimer = 0.0f;
	m_isAttackReady = true;

	m_AttackEffectTime = 0.4f;
	m_atkColTimer = 0.0f;

	m_isUnderAttack = false;
	m_underAttackValidTime = 0.2f;
	m_underAttackTimer = 0.0f;

	m_evadeDistance = 300.0f;
	m_evadeProbability = 0.2f;
	m_attack1Probability = 0.8f;
	m_attack2Probability = 0.0f;
	m_attack3Probability = 0.0f;

}