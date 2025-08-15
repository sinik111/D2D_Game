#include "../D2DEngineLib/framework.h"

#include <limits>
#include <iostream>
#include <sstream>
#include <fstream>

#include <cmath>    
#include <random>   
#include <chrono>   
#include <thread>

#include <d2d1.h>

#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/PlayerInput.h"
#include "../D2DEngineLib/Animator.h"
#include "../D2DEngineLib/SceneManager.h"
#include "../D2DEngineLib/RigidBody2D.h"
#include "../D2DEngineLib/Physics.h"
#include "../D2DEngineLib/BoxCollider2D.h"
#include "../D2DEngineLib/TextRenderer.h"
#include "../D2DEngineLib/FSMContext.h"
#include "../D2DEngineLib/ConeCollider2D.h"
#include "../D2DEngineLib/CircleCollider.h"
#include "../D2DEngineLib/ResourceManager.h"
#include "../D2DEngineLib/json.hpp"
#include "../D2DEngineLib/Random.h"
#include "../D2DEngineLib/D2DRenderer.h"
#include "../D2DEngineLib/Camera.h"
#include "../D2DEngineLib/Component.h"
#include "../D2DEngineLib/SoundManager.h"

#include "HitStopController.h"

#include "EnemyOutOfArea.h"
#include "EnemyInArea.h"
#include "EnemyEngage.h"
#include "EnemyReturn.h"
#include "EnemyOnEvade.h"
#include "EnemyKnockdown.h"
#include "EnemyGrabbed.h"
#include "EnemyHitAndRun.h"
#include "EnemyEvadeAndRun.h"
#include "EnemySlowTurn.h"
#include "EnemyDead.h"
#include "EnemyOnAttack.h"
#include "EnemyOnAttackSecond.h"
#include "EnemyOnAttackThird.h"

#include "EnemyBaseAttack.h"

#include "EnemyBase.h"
#include "Player.h"

#include "GameDataManager.h"
#include "RecordScorePopup.h"

const float PI = 3.141592f;
const double PI_D = 3.14159265358979323846;

using nlohmann::json;

static int uniqueIdCounter = 0;

void EnemyBase::Initialize()
{
	LoadJson(L"StatData/EnemyZakoStat.json");
}

void EnemyBase::Start()
{

	m_animator = GetGameObject()->GetComponent<Animator>();
	m_rigidBody = GetGameObject()->GetComponent<RigidBody2D>();
	m_textRenderer = GetGameObject()->GetComponent<TextRenderer>();
	m_collider = GetGameObject()->GetComponent<BoxCollider2D>();
	m_circleCollider = GetGameObject()->GetComponent<CircleCollider>();

	m_enemyIA = std::make_unique<EnemyInteract>(this);
	m_context.gameObject = GetGameObject();

	m_context.animator = m_animator;
	m_context.transform = GetTransform();
	m_context.bitmapRenderer = GetGameObject()->GetComponent<BitmapRenderer>();
	m_context.bitmapRenderer->SetColor({ 2.0f, 2.0f, 2.0f, 1.0f });
	m_context.rigidBody2d = m_rigidBody;
	m_rigidBody->SetGravityScale(0.0f);

	m_context.boolParams[L"IsPlayerDead"] = m_isPlayerDead;
	m_context.boolParams[L"IsFindPlayer"] = m_isFindPlayer;

	AnimationInit();
	StateInit();
	ColliderInit();

	PositionInit(OriginPos(), OriginAngle());

	//디버그 텍스트 렌더러
	//m_context.textRenderer = m_textRenderer;
	//DebugTextInit();

	vSetting();		//스탯 외의 변수 초기화

}
void EnemyBase::FixedUpdate()
{
	if (m_isDead) return;

	RigidBodyUpdate();
}

void EnemyBase::Update()
{
	if (m_doDeleteThisObj)
	{
		Destroy(GetGameObject());
		return;
	}

	//DebugTextUpdate();

	CheckKnockdownPointReset();

	CheckCameraArea();

	CheckDead();

	CheckAndTransitState();

	CheckAndTransitAnimState();

	UpdateBossAnim();

	m_fsm.Update(m_context);

	UpdateAttackColliderPosition();

	UnderAttackManage();

	HitOverayManager();

	if (!IsKnockdown() && !IsGrabbed())
	{
		UpdateDirection();

		if (!IsAttackReady()) { AttackCoolCheck(); }

		if (IsKnockback())
		{
			UpdateKnockback();

			return;
		}
	}

	if (m_isMovingOnTime)
	{
		MoveOnTime(MyTime::DeltaTime());
		return;
	}

	CheckStopMoving();


}

void EnemyBase::OnDestroy()
{

}



void EnemyBase::OnRender()
{
	D2D1_COLOR_F color;

	color = D2D1::ColorF(D2D1::ColorF::Violet);

	Bounds bounds = Bounds::RectToWorldBounds(D2D1_RECT_F{ -20.0f, -20.0f, 20.0f, 20.0f }, GetTransform()->GetWorldMatrix());

	D2D1_RECT_F rect{
		bounds.GetMin().x,
		bounds.GetMin().y,
		bounds.GetMax().x,
		bounds.GetMax().y
	};

	//auto rigidBodyTransform = this->GetGameObject()->GetComponent<RigidBody2D>()->GetTransform();

	//Vector2 position = rigidBodyTransform->GetWorldPosition();

	//D2D1_MATRIX_3X2_F transformMatrix = D2D1::Matrix3x2F::Identity();

	//transformMatrix = transformMatrix * D2D1::Matrix3x2F::Translation(position.x, position.y);

	//D2DRenderer::Get()->GetDeviceContext()->SetTransform(transformMatrix);
	D2DRenderer::Get()->GetDeviceContext()->SetTransform(Camera::s_mainCamera->GetViewMatrix() * D2DRenderer::Get()->GetUnityMatrix());

	D2DRenderer::Get()->DrawRect(rect, color);

	//

}

//---------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------


void EnemyBase::AnimationInit()
{
	InitAnimPath(m_idleAnimPath, L"NormalEnemyIdle");
	InitAnimPath(m_attack1_1AnimPath, L"NormalEnemyHorizontalAttack1");
	InitAnimPath(m_attack1_2AnimPath, L"NormalEnemyHorizontalAttack2");
	InitAnimPath(m_attack2_1AnimPath, L"NormalEnemyHorizontalAttack1");
	InitAnimPath(m_KnockdownAnimPath, L"NormalEnemyKnockDown");
	InitAnimPath(m_moveAnimPath, L"NormalEnemyMove");
	InitAnimPath(m_dieAnimPath, L"NormalEnemyDie");

	SetAnimState(EnemyAnimIdle);
}


void EnemyBase::ColliderInit()
{
	m_collider->SetSize({ 40.0f, 80.0f });
	m_collider->SetOffset({ 0.0f, 0.0f });
	m_collider->SetTrigger(false);
	m_collider->SetLayer(CollisionLayer::EnemyHitBox);

	m_circleCollider->SetOffset({ 0.0f, -45.0f });
	m_circleCollider->SetRadius(30.0f);
	m_circleCollider->SetLayer(CollisionLayer::EnemyMove);
}

void EnemyBase::PositionInit(Vector2& pos, float angle)
{
	m_rigidBody->SetPosition(pos);
	SetRotationAngle(angle);
}

void EnemyBase::StateInit()
{
	m_fsm.AddState<EnemyOutOfArea>(L"OutOfArea", false, this);
	m_fsm.AddState<EnemyInArea>(L"InArea", false, this);
	m_fsm.AddState<EnemyEngage>(L"Engage", false, this);
	m_fsm.AddState<EnemyReturn>(L"Return", false, this);
	m_fsm.AddState<EnemyOnEvade>(L"OnEvade", false, this);
	m_fsm.AddState<EnemyKnockdown>(L"Knockdown", false, this);
	m_fsm.AddState<EnemyGrabbed>(L"Grabbed", false, this);
	m_fsm.AddState<EnemyHitAndRun>(L"HitAndRun", false, this);
	m_fsm.AddState<EnemyEvadeAndRun>(L"EvadeAndRun", false, this);
	m_fsm.AddState<EnemySlowTurn>(L"SlowTurn", false, this);
	m_fsm.AddState<EnemyDead>(L"Dead", false, this);
	m_fsm.AddState<EnemyOnAttack>(L"OnAttack", false, this);
	m_fsm.AddState<EnemyOnAttackSecond>(L"OnAttack2", false, this);
	m_fsm.AddState<EnemyOnAttackThird>(L"OnAttack3", false, this);

	m_fsm.SetState(L"OutOfArea", m_context);
}


void EnemyBase::DebugTextInit()
{
	m_stateText = L"IdleState : 2";
	m_attackRangeText = L"Atk Range : NONE";
	m_distanceText = L"Distance : NONE";

	m_textRenderer->SetText(m_stateText + L" \n" + m_attackRangeText + L" \n" + m_distanceText);
	m_textRenderer->SetFontSize(20.f);
	m_textRenderer->SetHorizontalAlignment(HorizontalAlignment::Left);
	m_textRenderer->SetVerticalAlignment(VerticalAlignment::Center);
	m_textRenderer->SetRectSize({ 250.0f, 60.0f });
	m_textRenderer->SetSpaceType(SpaceType::World);
	m_textRenderer->SetSortOrder(10);
	m_textRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
}


void EnemyBase::DebugTextUpdate()
{
	m_stateText = m_context.currentStateName + L" " + std::to_wstring(CurrDir());

	if (m_textRenderer)
		m_textRenderer->SetText(m_stateText + L" \n" + m_attackRangeText + L" \n" + m_distanceText);
}




//------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------




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
		m_context.nextStateName = L"OutOfArea";
		break;

	case INAREA:
		m_context.nextStateName = L"InArea";
		break;

	case ENGAGE:
		m_context.nextStateName = L"Engage";
		break;

	case RETURN:
		m_context.nextStateName = L"Return";
		break;

	case ONEVADE:
		m_context.nextStateName = L"OnEvade";
		break;

	case KNOCKDOWN:
		m_context.nextStateName = L"Knockdown";
		break;

	case GRABBED:
		m_context.nextStateName = L"Grabbed";
		break;

	case HITANDRUN:
		m_context.nextStateName = L"HitAndRun";
		break;

	case EVADEANDRUN:
		m_context.nextStateName = L"EvadeAndRun";
		break;

	case SLOWTURN:
		m_context.nextStateName = L"SlowTurn";
		break;

	case DEAD:
		m_context.nextStateName = L"Dead";
		break;


	case ONATTACK:
		m_context.nextStateName = L"OnAttack";
		break;

	case ONATTACK2:
		m_context.nextStateName = L"OnAttack2";
		break;

	case ONATTACK3:
		m_context.nextStateName = L"OnAttack3";
		break;

	}

	m_context.shouldChangeState = true;

	m_nextEnemyState = NOSTATE;
	m_context.intParams[L"NextEnemyState"] = m_nextEnemyState; //NOSTATE

}


void EnemyBase::CheckAndTransitAnimState()
{
	if (m_currAnimationState != EnemyAnimIdle)
		SetAnimState(EnemyAnimIdle);
}

void EnemyBase::SetTargetPlayerAndCamera(GameObject* player, GameObject* camera)
{
	m_player = player;
	m_camera = camera;
}



void EnemyBase::UpdateBossAnim()
{
	if (m_enemyType != BOSS)
		return;

	// m_toDoMove 상태 변화 감지
	if (m_bossMoveCheck != m_toDoMove)
	{
		m_bossMoveCheck = m_toDoMove;

		if (m_toDoMove) // 이동을 시작할 때
		{
			m_nextAnimationState = EnemyAnimMove;
			//// 이동 거리가 2500보다 작으면 MoveIn 애니메이션 스킵
			//if (m_distanceToMoveSq < 2500.0f)
			//{
			//	m_nextAnimationState = EnemyAnimMove;
			//}
			//else
			//{
			//	m_nextAnimationState = EnemyAnimMoveIn;
			//	// m_bossMoveInTimer를 0으로 초기화하고, 애니메이션 재생이 끝났는지 체크할 로직 추가 필요
			//}
		}
		else // 이동을 멈출 때
		{
			m_nextAnimationState = EnemyAnimIdle;
			// m_bossMoveOutTimer를 0으로 초기화하고, 애니메이션 재생이 끝났는지 체크할 로직 추가 필요
		}
	}

	// --- 애니메이션 재생 시간 체크 로직 추가 ---
	// 이 부분은 Update 함수에서 매 프레임마다 호출되어야 합니다.
	// 현재 애니메이션이 MoveIn일 때
	//if (m_bossMoveCheck && m_currAnimationState == EnemyAnimMoveIn)
	//{
	//	m_bossMoveInTimer += MyTime::DeltaTime();
	//	if (m_bossMoveInTimer >= m_bossMoveInDuration)
	//	{
	//		SetAnimState(EnemyAnimMove);
	//		m_bossMoveInTimer = 0.0f; // 타이머 초기화
	//	}
	//}
	//// 현재 애니메이션이 MoveOut일 때
	//else if (!m_bossMoveCheck && m_currAnimationState == EnemyAnimMoveOut)
	//{
	//	m_bossMoveOutTimer += MyTime::DeltaTime();
	//	if (m_bossMoveOutTimer >= m_bossMoveOutDuration)
	//	{
	//		SetAnimState(EnemyAnimIdle);
	//		m_bossMoveOutTimer = 0.0f; // 타이머 초기화
	//	}
	//}
}

void EnemyBase::CheckCameraArea() {

	//카메라 범위 안에 있는지 체크하는 조건. 맞는지 확인 필요.
	Vector2 camPos = CameraPos();
	Vector2 myPos = Pos();
	Vector2 offset = Vector2(0.0f, 250.0f);

	if (myPos.x < camPos.x + 960.0f &&
		myPos.x > camPos.x - 960.0f &&
		myPos.y < camPos.y + 540.0f &&
		myPos.y > camPos.y - 540.0f - offset.y)
	{
		IsInCamera() = true;
		return;
	}

	IsInCamera() = false;
}


// FixedUpdate -------------------------------------------------------------------------------

void EnemyBase::RigidBodyUpdate()
{
	Vector2 desiredVelocity = m_aheadDirection * MoveSpeed();

	const float yAxisVisualCorrectionFactor = 0.5f;

	desiredVelocity.y *= yAxisVisualCorrectionFactor;

	Vector2 currentVelocity = m_rigidBody->GetVelocity();

	Vector2 steeringForce = (desiredVelocity - currentVelocity) * m_acceleration;

	const float maxSteeringForce = 2000.0f;

	if (steeringForce.LengthSq() > maxSteeringForce * maxSteeringForce)
	{
		steeringForce = steeringForce.Normalized() * maxSteeringForce;
	}

	m_rigidBody->AddForce(steeringForce);

	m_debugLogTimer += MyTime::FixedDeltaTime();

}

////---------------------------------------------------------------------------------------------------------------------------
//이동 관련//--------------------------------------------------------------------------------------------------------------------

void EnemyBase::CheckStopMoving()
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

void EnemyBase::MoveTo(const Vector2& destination)
{
	const Vector2 currentPos = Pos();

	DepaturePos() = currentPos;

	Vector2 direction = destination - currentPos;
	Vector2 dir = direction;

	if (direction.LengthSq() > 0)
	{
		direction.Normalize();
		m_aheadDirection = direction;
	}

	SetAngleByAheadDirection(m_aheadDirection);

	DistanceToMoveSq() = dir.LengthSq();

	MovingDestPos() = destination;

	ToDoMove() = true;


	m_acceleration = 200.0f;
}



void EnemyBase::MoveOnTime(float deltaTime)
{
	if (!m_isMovingOnTime) return;

	if (m_currEnemyState != ONATTACK && m_currEnemyState != ONATTACK2 &&
		m_currEnemyState != ONATTACK3 && m_currEnemyState != DEAD
		&& !m_isKnockdown && !m_isKnockback && m_currEnemyState != GRABBED
		&& m_currAnimationState != EnemyAnimMove)
	{
		//Debug::Log("enemyidleanim");
		//m_nextAnimationState = EnemyAnimIdle;
		m_nextAnimationState = EnemyAnimMove;
	}

	if (m_isOnEvade)
	{
		float tDiv = 0.3f;
		float tRotationSpeed = 80.0f;
		float tEndRotationSpeed = 150.0f;

		if (m_evadeType == 2)
		{
			tDiv = 0.6f;
			tRotationSpeed = 30.0f;
			tEndRotationSpeed = 180.0f;
		}

		m_moveOnTimeTimer += deltaTime;

		if (m_moveOnTimeTimer >= m_moveOnTimeDuration)
		{
			m_evadeType = 0;
			m_isMovingOnTime = false;
			m_isOnEvade = false;
			StopMoving();
			return;
		}

		if (m_moveOnTimeTimer > tDiv)
		{
			tRotationSpeed = tEndRotationSpeed;

			MoveSpeed() = m_moveSpeedEvade;
			m_acceleration = 250.0f;
		}

		float angleToAdd = tRotationSpeed * deltaTime * m_oddEvadeDirection;

		SetRotationAngle(m_rotationAngle + angleToAdd);

		float angleRad = m_rotationAngle * (PI / 180.0f);

		Vector2 newDirection(cosf(angleRad), sinf(angleRad));
		m_aheadDirection = newDirection;
	}
}



void EnemyBase::Evade(int evadeType = 0)
{
	StopMoving();

	const Vector2 currentPos = Pos();
	const Vector2 targetPos = TargetPos();

	Vector2 toMeFromPlayer = currentPos - targetPos;

	if (toMeFromPlayer.LengthSq() <= MyMath::EPSILON)
	{
		toMeFromPlayer = m_aheadDirection * -1.0f;
	}

	toMeFromPlayer.Normalize();

	m_evadeType = evadeType;
	m_oddEvadeDirection = Random::Int(0, 1) == 0 ? 1.0f : -1.0f;

	if (evadeType == 2)
	{
		Vector2 toPlayerFromOrigin = targetPos - OriginPos();

		if (toPlayerFromOrigin.LengthSq() <= MyMath::EPSILON)
		{
			MoveTo(currentPos + (m_aheadDirection * -1.0f) * m_evadeDistance);
			m_acceleration = 250.0f;
			return;
		}

		toPlayerFromOrigin.Normalize();

		Vector2 toMeFromOrigin = currentPos - OriginPos();
		if (toMeFromOrigin.LengthSq() <= MyMath::EPSILON)
		{
			MoveTo(currentPos + (m_aheadDirection * -1.0f) * m_evadeDistance);
			m_acceleration = 250.0f;
			return;
		}

		toMeFromOrigin.Normalize();

		Vector2 initialDirection = toPlayerFromOrigin * -1.0f;

		float tAngle = 30.0f;

		// 외적을 사용하여 회전 방향 결정		
		// 이 값이 양수면 시계 반대 방향, 음수면 시계 방향 회전이 더 가까움
		float crossProduct = Vector2::Cross(toMeFromOrigin, toPlayerFromOrigin);

		m_oddEvadeDirection = crossProduct > 0 ? -1.0f : 1.0f;

		tAngle *= -m_oddEvadeDirection;

		float angleRad = tAngle * (PI / 180.0f);

		float rotatedX = initialDirection.x * cosf(angleRad) - initialDirection.y * sinf(angleRad);
		float rotatedY = initialDirection.x * sinf(angleRad) + initialDirection.y * cosf(angleRad);

		m_aheadDirection = Vector2(rotatedX, rotatedY).Normalized();

	}
	else // evadeType이 0 또는 1일 경우 기존 로직 사용
	{
		m_oddEvadeDirection = Random::Int(0, 1) == 0 ? 1.0f : -1.0f;
		m_aheadDirection *= -1.0f;
	}

	if (evadeType != 0)
	{
		SetAngleByAheadDirection(m_aheadDirection);
	}

	if (evadeType == 0)
	{
		// 타입 0: 직선 회피 
		MoveTo(currentPos + toMeFromPlayer * m_evadeDistance);
		m_acceleration = 250.0f;
	}

	else if (evadeType == 1)
	{
		// 타입 1: 곡선 회피 초기화
		m_moveOnTimeDuration = 1.0f;
		m_moveOnTimeTimer = 0.0f;

		m_isMovingOnTime = true;
		ToDoMove() = true;
		MoveSpeed() = m_moveSpeedEvade * 1.5f;
		m_acceleration = 1000.0f;
	}
	else if (evadeType == 2)
	{
		// 타입 2: 곡선 회피 초기화
		m_moveOnTimeDuration = 1.3f;
		m_moveOnTimeTimer = 0.0f;

		m_isMovingOnTime = true;
		ToDoMove() = true;
		MoveSpeed() = m_moveSpeedEvade * 1.3f;
		m_acceleration = 2000.0f;
	}

	m_isOnEvade = true;
}


void EnemyBase::SetRotationAngle(float angle)
{
	m_rotationAngle = fmodf(angle, 360.0f);

	if (m_rotationAngle < 0.0f) {
		m_rotationAngle += 360.0f;
	}
}


void EnemyBase::AheadToTarget()
{
	Vector2 direction = TargetPos() - Pos();

	if (direction.LengthSq() <= MyMath::EPSILON)
		return;

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
	MoveSpeed() = m_moveSpeedOrigin;

	if (m_currEnemyState != ONATTACK && m_currEnemyState != ONATTACK2 &&
		m_currEnemyState != ONATTACK3 && m_currEnemyState != DEAD
		&& !m_isKnockdown && !m_isKnockback && m_currEnemyState != GRABBED
		&& m_currAnimationState != EnemyAnimIdle)
	{
		//Debug::Log("enemyidleanim");
		m_nextAnimationState = EnemyAnimIdle;
	}
}


void EnemyBase::SetAnimState(int animState)
{
	AnimPathArray temp;

	switch (animState)
	{
	case EnemyAnimIdle:
		temp = m_idleAnimPath;
		break;
	case EnemyAnimAttack1_1:
		temp = m_attack1_1AnimPath;
		break;
	case EnemyAnimAttack1_2:
		temp = m_attack1_2AnimPath;
		break;
	case EnemyAnimAttack2_1:
		temp = m_attack1_2AnimPath;
		break;
	case EnemyAnimKnockdown:
		temp = m_KnockdownAnimPath;
		break;
	case EnemyAnimMove:
		temp = m_moveAnimPath;
		break;
	case EnemyAnimDie:
		temp = m_dieAnimPath;
		break;
	}

	for (size_t i = 0; i < m_animPath.size(); ++i)
	{
		if (i == 0 || i == 5)
		{
			m_animPath[i] = temp[2];
			continue;
		}
		m_animPath[i] = temp[i];
	}
}

void EnemyBase::SetAnimDirection()
{
	CurrDir() = Direction();


	if (CurrDir() != PrevDir())
	{
		PrevDir() = CurrDir();

		size_t currentFrame = m_context.animator->GetCurrentFrame();

		m_animator->Play(m_animPath[CurrDir()], currentFrame);
	}
}

void EnemyBase::InitAnimPath(AnimPathArray& animPath, const std::wstring& animName)
{
	for (size_t i = 0; i < animPath.size(); ++i)
	{
		if (i == 0 || i == 5)
			continue;

		std::wstring baseFileName = animName + L"_" + m_animPathdir[i] + L"_001";

		animPath[i] = baseFileName;
		//animName + L"/" +
		std::wstring spriteSheetPath = animName + L"\\" + baseFileName + L"_sprites.json";
		m_animator->AddSpriteSheet(spriteSheetPath);

		std::wstring animationClipPath = animName + L"\\" + baseFileName + L"_anim.json";
		m_animator->AddAnimationClip(animationClipPath);
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
	AttackTimer() += MyTime::DeltaTime();

	if (AttackInterval() <= AttackTimer())
	{
		IsAttackReady() = true;
		AttackTimer() = 0.0f;
	}
}

EnemyBase::EnemyBase()
{
	m_uniqueId = uniqueIdCounter++;
}

void EnemyBase::UpdateAttackColliderPosition()
{
	Vector2 tLocalPos = this->GetTransform()->GetLocalPosition();
	tLocalPos.y += 30.0f;
	tLocalPos += (m_aheadDirection * 10.0f);
	m_attackColliderPosition = tLocalPos;
}




///////////-------------------------------------------------------------------------------------------------------------------------------
//피격관련//-------------------------------------------------------------------------------------------------------------------------------

void EnemyBase::UnderAttackManage()
{
	if (!m_isUnderAttack)
		return;

	m_underAttackTimer += MyTime::DeltaTime();

	if (m_underAttackTimer >= m_underAttackValidTime)
	{
		m_underAttackTimer = 0.0f;
		m_isUnderAttack = false;
		Debug::Log("체크히트!!");
		CheckEnemyHit();
	}
}

void EnemyBase::EnemyUnderAttack()
{
	Debug::Log("언더어택");

	m_hitOverayTimer = 0.0f;
	m_isHitOverayDraw = true;
	m_context.bitmapRenderer->SetColor({ 4.0f, 4.0f, 4.0f, 2.0f });

	auto controller = Camera::s_mainCamera->GetGameObject()->GetComponent<HitStopController>();
	controller->DoHitStop(0.01f, 0.1f);

	KdPointResetTimer() = 0.0f;
	IsUnderAttack() = true;
	UnderAttackTimer() = 0.0f;
}

void EnemyBase::HitOverayManager()
{
	if (m_isHitOverayDraw)
	{
		m_hitOverayTimer += MyTime::DeltaTime();

		if (m_hitOverayTimer >= m_hitOverayDuration)
		{
			m_context.bitmapRenderer->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			m_isHitOverayDraw = false;
			m_hitOverayTimer = 0.0f;
		}
	}
}

void EnemyBase::CheckEnemyHit()
{
	if (m_isDead || m_hp <= 0) return;

	int knockdownPow = 0;
	int dmg = 0;


	if (m_isGetDealed || m_isGetParried)
	{
		knockdownPow = m_player->GetComponent<Player>()->GetPlayerStat().attackKnockdownPower;

		dmg = Random::Int(m_player->GetComponent<Player>()->GetPlayerStat().attackPowerMin, m_player->GetComponent<Player>()->GetPlayerStat().attackPowerMax);
	}
	else if (m_isGetDealedHeavy || m_isGetParriedHeavy)
	{
		knockdownPow = m_player->GetComponent<Player>()->GetPlayerStat().heavyAttackKnockdownPower;

		dmg = Random::Int(m_player->GetComponent<Player>()->GetPlayerStat().heavyAttackPowerMin, m_player->GetComponent<Player>()->GetPlayerStat().heavyAttackPowerMax);
	}
	else
	{
		return;
	}

	m_hp -= dmg;



	if (!m_isKnockdown)
	{
		m_knockdownAccumulated += knockdownPow;
	}

	if (m_knockdownAccumulated > m_knockbackResist)
	{
		KnockBack(250.0f, 6.0f);
	}
	else
	{
		KnockBack(30.0f, 4.0f);
	}

	if (m_knockdownAccumulated >= m_knockdownResist)
	{
		m_isKnockdown = true;
	}
}



void EnemyBase::KnockBack(float dist = 10.0f, float speed = 5.0f)
{
	if (IsKnockback())
	{
		StopMoving();
	}

	IsKnockback() = true;
	KnockbackCurrentTimer() = 0.0f;


	const Vector2 enemyPos = Pos();
	const Vector2 playerPos = TargetPos();
	Vector2 knockBackDirection = enemyPos - playerPos;

	knockBackDirection.Normalize();
	KnockbackDirection() = knockBackDirection;

	AheadDirection() = knockBackDirection * -1.0f;
	SetAngleByAheadDirection(AheadDirection());

	KnockbackSpeed() = dist * speed;
}




void EnemyBase::CheckKnockdownPointReset()
{
	if (m_knockdownAccumulated > 0)
	{
		m_kdPointResetTimer += MyTime::DeltaTime();

		if (m_kdPointResetTimer >= m_knockdownPointResetTime)
		{
			m_knockdownAccumulated = 0;
			m_kdPointResetTimer = 0.0f;
		}
	}
}


void EnemyBase::CheckGrabbed()
{
	if (m_isGrabbed)
	{
		return;
	}
}




void EnemyBase::CheckDead()
{
	//if (m_isDead) return;

	if (m_hp <= 0)
	{
		m_isDead = true;

		//m_collider->UnregisterFromSystem();
		//m_collider = nullptr;


		if (m_context.intParams[L"CurrEnemyState"] != EnemyBase::DEAD)
		{
			m_context.intParams[L"NextEnemyState"] = EnemyBase::DEAD;

			switch (m_enemyType)
			{
			case TypeOfEnemy::ZAKO:
			{
				int score = GameDataManager::Get().GetPlayerScore();
				GameDataManager::Get().SetPlayerScore(score + 1000);

				std::wstring name = L"Enemy" + std::to_wstring(m_uniqueId);
				SoundManager::Get().StopSound(name);
			}
			break;

			case TypeOfEnemy::ELITE:
			{
				int score = GameDataManager::Get().GetPlayerScore();
				GameDataManager::Get().SetPlayerScore(score + 2500);

				std::wstring name = L"Enemy" + std::to_wstring(m_uniqueId);
				SoundManager::Get().StopSound(name);
			}
			break;

			case TypeOfEnemy::BOSS:
			{
				int score = GameDataManager::Get().GetPlayerScore();
				GameDataManager::Get().SetPlayerScore(score + 5000);

				SoundManager::Get().StopSound(L"BossIdle");

				Vector2 position = GetTransform()->GetLocalPosition();
				SoundManager::Get().PlaySound(L"BossDead", SoundType::SFX, &position);

				SoundManager::Get().StopSound(L"GamePlayBGM");

				// play clear bgm

				GameObject* popup = CreateGameObject(L"RecordPopup");
				popup->AddComponent<RecordScorePopup>();
			}
			break;
			}

			int killCount = GameDataManager::Get().GetPlayerKillCount();
			GameDataManager::Get().SetPlayerKillCount(++killCount);
		}

		return;
	}
}

void EnemyBase::StopKnockback()
{
	IsKnockback() = false;
	m_knockbackCurrentTimer = 0.0f;
	m_knockbackSpeed = 0.0f;
}


void EnemyBase::UpdateKnockback()
{
	if (!m_rigidBody) return;

	m_knockbackCurrentTimer += MyTime::DeltaTime();

	// 넉백 종료 조건
	if (m_knockbackCurrentTimer >= m_knockbackDuration)
	{
		StopMoving();
		StopKnockback();

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

}

void EnemyBase::OnTriggerExit(const Collision& collision)
{
	Debug::Log(L"EnemyBase OnTriggerExit");
}


void EnemyBase::SetAttackType(bool& isOnKnockback, bool& isTargetInRoamDist)
{
	m_attackType = 0;
	float tProb = 0;

	if (!isTargetInRoamDist || isOnKnockback)
	{
		tProb = Random::Float(1.0f, 80.0f);
		if (tProb <= m_attack3Probability) { m_attackType = 3; }
		else if (tProb <= m_attack2Probability) { m_attackType = 2; }
		else { m_attackType = 1; }

	}
	else
	{
		tProb = Random::Float(1.0f, 100.0f);
		if (tProb <= m_evadeProbability) { m_attackType = 4; }
		else if (tProb <= m_attack3Probability) { m_attackType = 3; }
		else if (tProb <= m_attack2Probability) { m_attackType = 2; }
		else { m_attackType = 1; }

	}
}

void EnemyBase::AssaultMove()
{
	const Vector2 currentPos = Pos();
	const Vector2 targetPos = TargetPos();

	Vector2 toMeFromTarget = currentPos - targetPos;
	if (toMeFromTarget.LengthSq() <= MyMath::EPSILON)
	{
		return;
	}

	toMeFromTarget.Normalize();

	Vector2 forwardPositionFromTarget = targetPos + (toMeFromTarget * 50.0f);

	Vector2 moveDestination;

	if (Vector2::Dot(toMeFromTarget, m_aheadDirection) < 0)
	{
		moveDestination = currentPos;
	}
	else
	{
		float distanceToForwardPos = Vector2::Distance(currentPos, forwardPositionFromTarget);
		float actualMoveDistance = distanceToForwardPos > m_attack1AMoveDist ? m_attack1AMoveDist : distanceToForwardPos;

		Vector2 moveDirection = forwardPositionFromTarget - currentPos;
		moveDirection.Normalize();

		moveDestination = currentPos + (moveDirection * actualMoveDistance);
	}

	MoveTo(moveDestination);

	MoveSpeed() = m_attack1AMoveSpd;
	m_acceleration = m_attack1AMoveSpd;
}


void EnemyBase::EnemyAttackTypeA_A()
{
	AssaultMove();

	m_attackWaitTimer = 0.0f;
	m_isAttackWait = true;

	m_nextAnimationState = EnemyAnimAttack1_1;


	int dmg = Random::Int(m_AttackPowerMin, m_AttackPowerMax);
	int kn = m_AttackKnockdownPower;

	m_enemyAttack = this->CreateGameObject(L"EnemyBaseAttack");
	auto comp = m_enemyAttack->AddComponent<EnemyBaseAttack>(this, m_attackDuration_1A, m_attackRadius_1A, m_attackAngle_1A, 0.45f, 0.9f, dmg, kn);
}

void EnemyBase::EnemyAttackTypeA_B()
{
	AssaultMove();

	m_attackWaitTimer = 0.0f;
	m_isAttackWait = true;

	int dmg = Random::Int(m_AttackPowerMin, m_AttackPowerMax);
	int kn = m_AttackKnockdownPower;

	m_nextAnimationState = EnemyAnimAttack1_2;

	m_enemyAttack = this->CreateGameObject(L"EnemyBaseAttack");
	auto comp = m_enemyAttack->AddComponent<EnemyBaseAttack>(this, m_attackDuration_1B, m_attackRadius_1B, m_attackAngle_1B, 0.45f, 0.9f, dmg, kn);
}


void EnemyBase::EnemyAttackTypeA_C()
{
	AssaultMove();

	m_attackWaitTimer = 0.0f;
	m_isAttackWait = true;

	int dmg = Random::Int(m_AttackPowerMin, m_AttackPowerMax);
	int kn = m_AttackKnockdownPower;

	m_enemyAttack = this->CreateGameObject(L"EnemyBaseAttack");
	auto comp = m_enemyAttack->AddComponent<EnemyBaseAttack>(this, m_attackDuration_1C, m_attackRadius_1C, m_attackAngle_1C, 0.45f, 0.9f, dmg, kn);
}

void EnemyBase::EnemyAttackTypeB_A()
{
	AssaultMove();

	m_attackWaitTimer = 0.0f;
	m_isAttackWait = true;

	int dmg = Random::Int(m_Attack2PowerMin, m_Attack2PowerMax);
	int kn = m_AttackKnockdownPower;

	m_nextAnimationState = EnemyAnimAttack2_1;

	m_enemyAttack = this->CreateGameObject(L"EnemyBaseAttack");
	auto comp = m_enemyAttack->AddComponent<EnemyBaseAttack>(this, m_attackDuration_2A, m_attackRadius_2A, m_attackAngle_2A, 0.45f, 0.9f, dmg, kn);
}

void EnemyBase::EnemyAttackTypeB_B()
{
	AssaultMove();

	m_attackWaitTimer = 0.0f;
	m_isAttackWait = true;

	int dmg = Random::Int(m_Attack2PowerMin, m_Attack2PowerMax);
	int kn = m_AttackKnockdownPower;

	m_nextAnimationState = EnemyAnimAttack2_1;

	m_enemyAttack = this->CreateGameObject(L"EnemyBaseAttack");
	auto comp = m_enemyAttack->AddComponent<EnemyBaseAttack>(this, m_attackDuration_2B, m_attackRadius_2B, m_attackAngle_2B, 0.45f, 0.9f, dmg, kn);
}

void EnemyBase::EnemyAttackTypeB_C()
{
	AssaultMove();

	m_attackWaitTimer = 0.0f;
	m_isAttackWait = true;

	int dmg = Random::Int(m_Attack2PowerMin, m_Attack2PowerMax);
	int kn = m_AttackKnockdownPower;

	m_enemyAttack = this->CreateGameObject(L"EnemyBaseAttack");
	auto comp = m_enemyAttack->AddComponent<EnemyBaseAttack>(this, m_attackDuration_2C, m_attackRadius_2C, m_attackAngle_2C, 0.45f, 0.9f, dmg, kn);
}

void EnemyBase::EnemyAttackTypeC_A()
{
	AssaultMove();

	m_attackWaitTimer = 0.0f;
	m_isAttackWait = true;

	int dmg = Random::Int(m_Attack3PowerMin, m_Attack3PowerMax);
	int kn = m_AttackKnockdownPower;

	m_enemyAttack = this->CreateGameObject(L"EnemyBaseAttack");
	auto comp = m_enemyAttack->AddComponent<EnemyBaseAttack>(this, m_attackDuration_3A, m_attackRadius_3A, m_attackAngle_3A, 0.45f, 0.9f, dmg, kn);
}

void EnemyBase::EnemyAttackTypeC_B()
{
	AssaultMove();

	m_attackWaitTimer = 0.0f;
	m_isAttackWait = true;

	int dmg = Random::Int(m_Attack3PowerMin, m_Attack3PowerMax);
	int kn = m_AttackKnockdownPower;

	m_enemyAttack = this->CreateGameObject(L"EnemyBaseAttack");
	auto comp = m_enemyAttack->AddComponent<EnemyBaseAttack>(this, m_attackDuration_3B, m_attackRadius_3B, m_attackAngle_3B, 0.45f, 0.9f, dmg, kn);
}

void EnemyBase::EnemyAttackTypeC_C()
{
	AssaultMove();

	m_attackWaitTimer = 0.0f;
	m_isAttackWait = true;

	int dmg = Random::Int(m_Attack3PowerMin, m_Attack3PowerMax);
	int kn = m_AttackKnockdownPower;

	m_enemyAttack = this->CreateGameObject(L"EnemyBaseAttack");
	auto comp = m_enemyAttack->AddComponent<EnemyBaseAttack>(this, m_attackDuration_3C, m_attackRadius_3C, m_attackAngle_3C, 0.45f, 0.9f, dmg, kn);
}


void EnemyBase::vSetting()
{
	m_knockbackDuration = 0.5f;

	m_grabTimer = 3.0f;

	m_slowRotationSpeed = 200.0f;

	//--------------------------------------------------

	m_isInCamera = false;
	m_isDead = false;

	m_prevEnemyState = NOSTATE;
	m_nextEnemyState = NOSTATE;
	m_currEnemyState = OUTOFAREA;

	m_nextAnimationState = NoAnimation;
	m_prevAnimationState = NoAnimation;
	m_currAnimationState = EnemyAnimIdle;

	m_context.intParams[L"PrevEnemyState"] = m_prevEnemyState;
	m_context.intParams[L"NextEnemyState"] = m_nextEnemyState;
	m_context.intParams[L"CurrEnemyState"] = m_currEnemyState;

	m_rotationAngle = 0.0f;
	m_direction = DOWN;
	m_prevDir = DOWN;
	m_currDir = DOWN;

	m_isPlayerDead = false;

	m_toDoMove = false;
	m_distanceToMoveSq = 0.0f;
	m_movingDestPos = Pos();
	m_departurePos = Pos();
	m_aheadDirection = Vector2(0.0f, -1.0f);

	m_acceleration = 0.0f;

	m_isLockOnTarget = false;
	m_isTargetInChaseDist = false;
	m_isTargetInRoamDist = false;
	m_isTargetInAtkRange = false;
	m_isTargetInMaxAtkRange = false;
	m_isTargetInAtkAngle = false;

	m_attackTimer = 0.0f;
	m_isAttackReady = true;
	m_attackType = 0;
	m_isInCombo = false;

	m_isUnderAttack = false;
	m_underAttackValidTime = 0.3f;
	m_underAttackTimer = 0.0f;

	m_isKnockdown = false;
	m_knockdownAccumulated = 0;
	m_kdPointResetTimer = 0.0f;

	m_isKnockback = false;
	m_knockbackDirection = Vector2(0.0f, 0.0f);
	m_knockbackSpeed = 0.0f;
	m_knockbackAcceleration = 0.0f;
	m_knockbackCurrentTimer = 0.0f;

	m_isGrabbed = false;

	m_isFindPlayer = false;

	m_slowRotationSpeed = 200.0f;

	m_doDeleteThisObj = false;

	//m_recorveryTime = 1.5f;

}


void EnemyBase::LoadJson(std::wstring path)
{
	std::ifstream inFile(ResourceManager::Get().GetResourcePath() + path);

	if (inFile.is_open())
	{
		json j;
		inFile >> j;
		inFile.close();

		m_maxHp = j["MaxHp"];
		m_hp = m_maxHp;
		m_moveSpeed = j["MoveSpeed"];
		m_moveSpeedOrigin = j["MoveSpeedOrigin"];
		m_moveSpeedEvade = j["MoveSpeedEvade"];
		m_maxSightDistance = j["MaxSightDistance"];
		m_sightAngle = j["SightAngle"];
		m_maxRoamDistance = j["MaxRoamDistance"];
		m_maxChaseDistance = j["MaxChaseDistance"];
		m_AttackRange = j["AttackRange"];
		m_atkAngle = j["AtkAngle"];
		m_attackInterval = j["AttackInterval"];
		m_knockdownResist = j["KnockdownResist"];
		m_knockdownPointResetTime = j["KnockdownPointResetTime"];
		m_knockbackResist = j["KnockbackResist"];

		m_evadeDistance = j["EvadeDistance"];
		m_evadeProbability = j["EvadeProbability"];
		m_attack1Probability = j["Attack1Probability"];
		m_attack2Probability = j["Attack2Probability"];
		m_attack3Probability = j["Attack3Probability"];

		m_AttackPowerMin = j["AttackPowerMin"];
		m_AttackPowerMax = j["AttackPowerMax"];
		m_Attack2PowerMin = j["Attack2PowerMin"];
		m_Attack2PowerMax = j["Attack2PowerMax"];
		m_Attack3PowerMin = j["Attack3PowerMin"];
		m_Attack3PowerMax = j["Attack3PowerMax"];
		m_AttackKnockdownPower = j["AttackKnockdownPower"];

		m_attackA_BProb = j["AttackA_BProb"];
		m_attackA_CProb = j["AttackA_CProb"];

		m_attackB_BProb = j["AttackB_BProb"];
		m_attackB_CProb = j["AttackB_CProb"];

		m_attackC_BProb = j["AttackC_BProb"];
		m_attackC_CProb = j["AttackC_CProb"];

		m_attackA_ATime = j["AttackA_ATime"];
		m_attackA_BTime = j["AttackA_BTime"];
		m_attackA_CTime = j["AttackA_CTime"];

		m_attackB_ATime = j["AttackB_ATime"];
		m_attackB_BTime = j["AttackB_BTime"];
		m_attackB_CTime = j["AttackB_CTime"];

		m_attackC_ATime = j["AttackC_ATime"];;
		m_attackC_BTime = j["AttackC_BTime"];
		m_attackC_CTime = j["AttackC_CTime"];

		m_attack1AMoveSpd = j["Attack1AMoveSpd"];
		m_attack1BMoveSpd = j["Attack1BMoveSpd"];
		m_attack1CMoveSpd = j["Attack1CMoveSpd"];
		m_attack2AMoveSpd = j["Attack2AMoveSpd"];
		m_attack2BMoveSpd = j["Attack2BMoveSpd"];
		m_attack2CMoveSpd = j["Attack2CMoveSpd"];
		m_attack3AMoveSpd = j["Attack3AMoveSpd"];
		m_attack3BMoveSpd = j["Attack3BMoveSpd"];
		m_attack3CMoveSpd = j["Attack3CMoveSpd"];
		m_attack1AMoveDist = j["Attack1AMoveDist"];
		m_attack1BMoveDist = j["Attack1BMoveDist"];
		m_attack1CMoveDist = j["Attack1CMoveDist"];
		m_attack2AMoveDist = j["Attack2AMoveDist"];
		m_attack2BMoveDist = j["Attack2BMoveDist"];
		m_attack2CMoveDist = j["Attack2CMoveDist"];
		m_attack3AMoveDist = j["Attack3AMoveDist"];
		m_attack3BMoveDist = j["Attack3BMoveDist"];
		m_attack3CMoveDist = j["Attack3CMoveDist"];

		m_attack1ASafetyTime = j["Attack1ASafetyTime"];
		m_attack1BSafetyTime = j["Attack1BSafetyTime"];
		m_attack1CSafetyTime = j["Attack1CSafetyTime"];
		m_attack2ASafetyTime = j["Attack2ASafetyTime"];
		m_attack2BSafetyTime = j["Attack2BSafetyTime"];
		m_attack2CSafetyTime = j["Attack2CSafetyTime"];
		m_attack3ASafetyTime = j["Attack3ASafetyTime"];
		m_attack3BSafetyTime = j["Attack3BSafetyTime"];
		m_attack3CSafetyTime = j["Attack3CSafetyTime"];

	}
	else
	{
		assert(false && L"json 파일 열기 실패");
	}
}
