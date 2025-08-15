#include "../D2DEngineLib/framework.h"

#include <limits>
#include <iostream>
#include <sstream>
#include <fstream>

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

#include "EnemyZako.h"

using nlohmann::json;

void EnemyZako::Initialize()
{
	OriginPos() = GetGameObject()->GetTransform()->GetLocalPosition();
	OriginAngle() = -90.0f;
	LoadJson(L"EnemyZakoStat.json");	
}


void EnemyZako::OnCollisionEnter(const Collision& collision)
{
	Debug::Log(L"EnemyZako OnCollisionEnter");
}

void EnemyZako::OnCollisionStay(const Collision& collision)
{
}

void EnemyZako::OnCollisionExit(const Collision& collision)
{
}

void EnemyZako::OnTriggerEnter(const Collision& collision)
{
	Debug::Log(L"EnemyZako OnTriggerEnter");
}

void EnemyZako::OnTriggerExit(const Collision& collision)
{
	Debug::Log(L"EnemyBase OnTriggerExit");
}



void EnemyZako::AnimationInit()
{
	InitAnimPath(m_idleAnimPath, L"EmemyIdle");
	InitAnimPath(m_attack1AnimPath, L"EnemyAtteck001");
	InitAnimPath(m_hitAnimPath, L"EmemyHit");

	SetAnimState(EnemyAnimIdle);
}


void EnemyZako::CheckAndTransitAnimState()
{	
	if (m_nextAnimationState != NoAnimation)
	{
		m_prevAnimationState = m_currAnimationState;
		m_currAnimationState = m_nextAnimationState;
		m_nextAnimationState = NoAnimation;

		SetAnimState(m_currAnimationState);
		m_animator->Play(m_animPath[CurrDir()]);
	}	
}

void EnemyZako::ColliderInit()
{
	m_collider->SetSize({ 30.0f, 50.0f });
	m_collider->SetOffset({ 0.0f, 60.0f });
	m_collider->SetTrigger(false);
	m_collider->SetLayer(CollisionLayer::EnemyHitBox);

	m_circleCollider->SetOffset({ 0.0f, 15.0f });
	m_circleCollider->SetRadius(35.0f);
	m_circleCollider->SetLayer(CollisionLayer::EnemyMove);
}


void EnemyZako::StateInit()
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


void EnemyZako::DebugTextInit()
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


void EnemyZako::DebugTextUpdate()
{
	m_stateText = m_context.currentStateName + L" " + std::to_wstring(CurrDir());

	if (m_textRenderer)
		m_textRenderer->SetText(m_stateText + L" \n" + m_attackRangeText + L" \n" + m_distanceText);
}

void EnemyZako::SetAnimState(int animState)
{
	AnimPathArray temp;

	switch (animState)
	{
	case EnemyAnimIdle:
		temp = m_idleAnimPath;
		break;
	case EnemyAnimAttack001:
		temp = m_attack1AnimPath;
		break;
	case EnemyAnimHit:
		temp = m_hitAnimPath;
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

void EnemyZako::CheckAndTransitState()
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
		m_nextAnimationState = EnemyAnimIdle;
		break;

	case INAREA:
		m_context.nextStateName = L"InArea";
		m_nextAnimationState = EnemyAnimIdle;
		break;

	case ENGAGE:
		m_context.nextStateName = L"Engage";
		m_nextAnimationState = EnemyAnimIdle;
		break;

	case RETURN:
		m_context.nextStateName = L"Return";
		m_nextAnimationState = EnemyAnimIdle;
		break;

	case ONEVADE:
		m_context.nextStateName = L"OnEvade";
		m_nextAnimationState = EnemyAnimIdle;
		break;

	case KNOCKDOWN:
		m_context.nextStateName = L"Knockdown";
		m_nextAnimationState = EnemyAnimIdle;
		break;

	case GRABBED:
		m_context.nextStateName = L"Grabbed";
		m_nextAnimationState = EnemyAnimIdle;
		break;

	case HITANDRUN:
		m_context.nextStateName = L"HitAndRun";
		m_nextAnimationState = EnemyAnimIdle;
		break;

	case EVADEANDRUN:
		m_context.nextStateName = L"EvadeAndRun";
		m_nextAnimationState = EnemyAnimIdle;
		break;

	case SLOWTURN:
		m_context.nextStateName = L"SlowTurn";
		m_nextAnimationState = EnemyAnimIdle;
		break;

	case DEAD:
		m_context.nextStateName = L"Dead";
		break;


	case ONATTACK:
		m_context.nextStateName = L"OnAttack";
		m_nextAnimationState = EnemyAnimAttack001;
		break;

	case ONATTACK2:
		m_context.nextStateName = L"OnAttack2";
		m_nextAnimationState = EnemyAnimAttack001;
		break;

	case ONATTACK3:
		m_context.nextStateName = L"OnAttack3";
		m_nextAnimationState = EnemyAnimAttack001;
		break;

	}

	m_context.shouldChangeState = true;

	m_nextEnemyState = NOSTATE;
	m_context.intParams[L"NextEnemyState"] = m_nextEnemyState; //NOSTATE
}

void EnemyZako::vSetting()
{
	m_knockbackDuration = 0.5f;

	m_grabTimer = 3.0f;

	m_slowRotationSpeed = 200.0f;

	//--------------------------------------------------

	m_enemyType = BASE;
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

	m_targetSlowRotationAngle = 0.0f;

	m_doDeleteThisObj = false;

	//m_recorveryTime = 1.5f;
}

void EnemyZako::Start()
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

	m_context.rigidBody2d = m_rigidBody;
	m_rigidBody->SetGravityScale(0.0f);

	m_context.boolParams[L"IsPlayerDead"] = m_isPlayerDead;
	m_context.boolParams[L"IsFindPlayer"] = m_isFindPlayer;

	AnimationInit();
	StateInit();
	ColliderInit();

	PositionInit(OriginPos(), OriginAngle());

	//디버그 텍스트 렌더러
	m_context.textRenderer = m_textRenderer;
	DebugTextInit();

	vSetting();		//스탯 외의 변수 초기화

}

