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

#include "TempEnemyAttackCollider.h"




void EnemyBase::Initialize()
{
	m_playerInput = GetGameObject()->GetComponent<PlayerInput>();
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
	//m_collider = GetGameObject()->GetComponent<BoxCollider2D>();

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

	PositionInit(500.0f, 0.0f, -90.0f);

	m_maxRoamDistance = 800.0f;
	m_maxChaseDistance = 900.0f;

	m_fsm.AddState<EnemyOutOfArea>(L"OutOfArea", false, this);
	m_fsm.AddState<EnemyInArea>(L"InArea", false, this);
	m_fsm.AddState<EnemyEngage>(L"Engage", false, this);
	m_fsm.AddState<EnemyOnAttack>(L"OnAttack", false, this);
	m_fsm.AddState<EnemyReturn>(L"Return", false, this);

	m_fsm.SetState(L"OutOfArea", m_context);

	m_context.intParams[L"NextEnemyState"] = NOSTATE;
	m_context.intParams[L"CurrEnemyState"] = OUTOFAREA;
	

	//콜라이더 세팅
	//m_collider->SetSize({ 30.0f, 45.0f });
	//m_collider->SetOffset({ 0.0f, 45.0f });

}
void EnemyBase::FixedUpdate()
{
	RigidBodyUpdate();
}

void EnemyBase::Update()
{	

	CheckAndTransitState();

	CheckCameraArea();	

	AtkColliderLifeManage();

	AttackCoolCheck();

	UnderAttackManage();


	m_fsm.Update(m_context);


	UpdateDirection();

	UpdateMovement();
}

void EnemyBase::OnDestroy()
{
	if (m_atkCollider != nullptr)		
	{
		Destroy(m_atkCollider);
		m_atkCollider = nullptr;		
	}
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

	m_context.shouldChangeState = true;

	m_currEnemyState = m_nextEnemyState;


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
	}

	m_nextEnemyState = NOSTATE;
	m_context.intParams[L"NextEnemyState"] = m_nextEnemyState;
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

	m_acceleration = 200.0f;
	
	const Vector2 currentPosition = Pos();
	const float distance = Vector2::Distance(MovingDestPos(), currentPosition);

	if (distance <= 2.0f)
	{		
		StopMoving();
		return;
	}
	
}


void EnemyBase::RigidBodyUpdate()
{
	Vector2 desiredVelocity = m_moveDirection * MoveSpeed();
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
	
	const float distance = Vector2::Distance(currentPos, destination);

	if (distance < MyMath::EPSILON)
	{
		return;
	}

	MovingDestPos() = destination;
	ToDoMove() = true;
	
	Vector2 direction = MovingDestPos() - currentPos;
		
	direction.Normalize();

	float targetAngleRadians = atan2f(direction.y, direction.x);

	float targetAngleDegrees = targetAngleRadians * (180.0f / 3.141592f);

	SetRotationAngle(targetAngleDegrees);
	
	m_moveDirection = direction;

}


void EnemyBase::SetRotationAngle(float angle)
{
	m_rotationAngle = angle;

	if (m_rotationAngle < 0.0f) m_rotationAngle += 360.0f;
}


void EnemyBase::StopMoving()
{
	ToDoMove() = false;
	m_rigidBody->SetVelocity(Vector2::Zero);
	m_acceleration = 0.0f;

	SetAngleByDirection(m_direction);
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


///////////-------------------------------------------------------------------------------------------------------------------------------
//공격관련//-------------------------------------------------------------------------------------------------------------------------------

void EnemyBase::FakeAttack()
{
	if (!m_isAttackReady)
		return;

	m_atkCollider = CreateGameObject(m_tempLabel);
	m_atkCollider->AddComponent<TextRenderer>();
	m_atkCollider->AddComponent<RigidBody2D>();
	m_atkCollider->AddComponent<BoxCollider2D>();
	m_atkCollider->AddComponent<TempEnemyAttackCollider>(this);
}

void EnemyBase::AtkColliderLifeManage()
{
	if (m_atkCollider == nullptr)
		return;

	if (m_atkColTimer >= m_AttackEffectTime)
	{
		m_atkColTimer = 0.0f;
		Destroy(m_atkCollider, 0.0f);
		m_atkCollider = nullptr;
		return;
	}

	m_atkColTimer += MyTime::DeltaTime();
}


void EnemyBase::AttackCoolCheck()
{
	if (m_isAttackReady)
		return;

	m_attackTimer += MyTime::DeltaTime();

	if (m_attackInterval <= m_attackTimer)
	{
		m_isAttackReady = true;
		m_attackTimer = 0.0f;
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
	m_AttackRange = 20.0f;

	m_isLockOnTarget = false;
	m_isTargetInChaseDist = false;
	m_isTargetInRoamDist = false;
	m_isTargetInAtkRange = false;

	m_atkAngle = 10.0f;
	m_isTargetInAtkAngle = false;

	m_attackInterval = 1.5f;
	m_attackTimer = 0.0f;
	m_isAttackReady = true;

	m_AttackEffectTime = 0.4f;
	m_atkColTimer = 0.0f;

	m_isUnderAttack = false;
	m_underAttackValidTime = 0.2f;
	m_underAttackTimer = 0.0f;
}