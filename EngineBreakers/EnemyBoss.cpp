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
#include "../D2DEngineLib/SoundManager.h"

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

#include "EnemyBoss.h"


using nlohmann::json;



void EnemyBoss::Initialize()
{
	OriginPos() = GetGameObject()->GetTransform()->GetLocalPosition();
	OriginAngle() = -90.0f;
	LoadJson(L"StatData/EnemyBossStat.json");
	
}


void EnemyBoss::OnCollisionEnter(const Collision& collision)
{
	Debug::Log(L"EnemyBoss OnCollisionEnter");
}

void EnemyBoss::OnCollisionStay(const Collision& collision)
{
}

void EnemyBoss::OnCollisionExit(const Collision& collision)
{
}

void EnemyBoss::OnTriggerEnter(const Collision& collision)
{
	Debug::Log(L"EnemyBoss OnTriggerEnter");
}

void EnemyBoss::OnTriggerExit(const Collision& collision)
{
	Debug::Log(L"EnemyBoss OnTriggerExit");
}



void EnemyBoss::AnimationInit()
{
	InitAnimPath(m_idleAnimPath, L"Boss Idle");	
	InitAnimPath(m_moveAnimPath, L"BossMove");	

	InitAnimPath(m_attack1_1AnimPath, L"BossAttack001");	
	InitAnimPath(m_attack2_1AnimPath, L"BossAttack002");

	InitAnimPath(m_KnockdownAnimPath, L"BossDown");
	
	InitAnimPath(m_dieAnimPath, L"BossDead");
	InitAnimPath(m_bossDashAnimPath, L"BossDash");
	InitAnimPath(m_bossMoveInAnimPath, L"BossMoveIn");
	InitAnimPath(m_bossMoveOutAnimPath, L"BossMoveOut");	

	m_animator->AddActionOnEvent(L"Footstep1", this, &EnemyBoss::Footstep1);
	m_animator->AddActionOnEvent(L"Footstep2", this, &EnemyBoss::Footstep2);
	m_animator->AddActionOnEvent(L"HorizontalAttack1", this, &EnemyBoss::HAttack1);
	m_animator->AddActionOnEvent(L"HorizontalAttack2", this, &EnemyBoss::HAttack2);
	m_animator->AddActionOnEvent(L"HorizontalAttack3", this, &EnemyBoss::HAttack3);
	m_animator->AddActionOnEvent(L"StabPreAttack", this, &EnemyBoss::SPreAttacking);
	m_animator->AddActionOnEvent(L"StabAttacking", this, &EnemyBoss::SAttacking);

	SetAnimState(EnemyAnimIdle);
}


void EnemyBoss::CheckAndTransitAnimState()
{
	if (m_nextAnimationState != NoAnimation)
	{
		m_prevAnimationState = m_currAnimationState;
		m_currAnimationState = m_nextAnimationState;
		m_nextAnimationState = NoAnimation;

		SetAnimState(m_currAnimationState);

		if (CurrDir() == 0 || CurrDir() == 5)
			return;

		m_animator->Play(m_animPath[CurrDir()]);
	}
}

void EnemyBoss::ColliderInit()
{
	m_collider->SetSize({ 30.0f, 50.0f });
	m_collider->SetOffset({ 0.0f, 60.0f });
	m_collider->SetTrigger(false);
	m_collider->SetLayer(CollisionLayer::EnemyHitBox);

	m_circleCollider->SetOffset({ 0.0f, 15.0f });
	m_circleCollider->SetRadius(35.0f);
	m_circleCollider->SetLayer(CollisionLayer::EnemyMove);
}


void EnemyBoss::InitAnimPath(AnimPathArray& animPath, const std::wstring& animName)
{
	if (animName == L"BossDown")
	{
		std::wstring spriteSheetPath = L"Boss\\BossDOWN\\BossDOWN_sprites.json";
		m_animator->AddSpriteSheet(spriteSheetPath);

		for (size_t i = 0; i < animPath.size(); ++i)
		{
			if (i == 0 || i == 5)
				continue;

			std::wstring baseFileName = L"BossDOWN_" + m_animPathdir[i];

			animPath[i] = baseFileName;

			std::wstring animationClipPath = L"Boss\\BossDOWN\\" + baseFileName + L"_anim.json";
			m_animator->AddAnimationClip(animationClipPath);
		}
	}
	else
	{
		std::wstring spriteSheetPath = L"Boss\\" + animName + L"\\" + animName + L"_sprites.json";
		m_animator->AddSpriteSheet(spriteSheetPath);

		for (size_t i = 0; i < animPath.size(); ++i)
		{
			if (i == 0 || i == 5)
				continue;

			std::wstring baseFileName = animName + L"_" + m_animPathdir[i];

			animPath[i] = baseFileName;

			std::wstring animationClipPath = L"Boss\\" + animName + L"\\" + baseFileName + L"_anim.json";
			m_animator->AddAnimationClip(animationClipPath);
		}
	}
}

void EnemyBoss::StateInit()
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


void EnemyBoss::DebugTextInit()
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


void EnemyBoss::DebugTextUpdate()
{
	m_stateText = m_context.currentStateName + L" " + std::to_wstring(CurrDir());

	if (m_textRenderer)
		m_textRenderer->SetText(m_stateText + L" \n" + m_attackRangeText + L" \n" + m_distanceText);
}


void EnemyBoss::CheckAndTransitState()
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
		m_nextAnimationState = EnemyAnimKnockdown;
		break;

	case GRABBED:
		m_context.nextStateName = L"Grabbed";
		m_nextAnimationState = EnemyAnimKnockdown;
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
		m_nextAnimationState = EnemyAnimDie;
		break;


	case ONATTACK:
		m_context.nextStateName = L"OnAttack";
		
		break;

	case ONATTACK2:	
		m_context.nextStateName = L"OnAttack2";
		m_nextAnimationState = EnemyAnimAttack2_1;
		break;

	//case ONATTACK3:
	//	m_context.nextStateName = L"OnAttack3";
	//	m_nextAnimationState = EnemyAnimAttack1_1;
	//	break;

	default:
		m_context.nextStateName = L"InArea";
		m_nextAnimationState = EnemyAnimIdle;
		break;

	}

	m_context.shouldChangeState = true;

	m_nextEnemyState = NOSTATE;
	m_context.intParams[L"NextEnemyState"] = m_nextEnemyState; //NOSTATE
}

void EnemyBoss::vSetting()
{
	m_knockbackDuration = 0.5f;

	m_grabTimer = 3.0f;

	m_slowRotationSpeed = 200.0f;

	//--------------------------------------------------

	m_enemyType = BOSS;
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

void EnemyBoss::SetAnimState(int animState)
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
		temp = m_attack1_1AnimPath;
		break;
	case EnemyAnimAttack2_1:
		temp = m_attack2_1AnimPath;
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
	case EnemyAnimMoveIn:
		temp = m_bossMoveInAnimPath;
		break;
	case EnemyAnimMoveOut:
		temp = m_bossMoveOutAnimPath;
		break;
	default:
		temp = m_idleAnimPath;
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

void EnemyBoss::Start()
{
	//GetGameObject()->GetTransform()->SetLocalScale({ 1.4f, 1.4f });

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
	//m_context.textRenderer = m_textRenderer;
	//DebugTextInit();

	vSetting();		//스탯 외의 변수 초기화

	Vector2 position = GetTransform()->GetLocalPosition();
	SoundManager::Get().PlaySound(L"BossIdle", SoundType::SFX, &position, L"BossIdle", true);
}

void EnemyBoss::Update()
{
	Vector2 position = GetTransform()->GetLocalPosition();
	SoundManager::Get().Update3DAttribute(L"BossIdle", position);

	EnemyBase::Update();
}

void EnemyBoss::Footstep1()
{
	Vector2 position = GetTransform()->GetLocalPosition();
	SoundManager::Get().PlaySound(L"BossFootstep1", SoundType::SFX, &position);
}

void EnemyBoss::Footstep2()
{
	Vector2 position = GetTransform()->GetLocalPosition();
	SoundManager::Get().PlaySound(L"BossFootstep2", SoundType::SFX, &position);
}

void EnemyBoss::HAttack1()
{
	Vector2 position = GetTransform()->GetLocalPosition();
	SoundManager::Get().PlaySound(L"BossHAttack1", SoundType::SFX, &position);
}

void EnemyBoss::HAttack2()
{
	Vector2 position = GetTransform()->GetLocalPosition();
	SoundManager::Get().PlaySound(L"BossHAttack2", SoundType::SFX, &position);
}

void EnemyBoss::HAttack3()
{
	Vector2 position = GetTransform()->GetLocalPosition();
	SoundManager::Get().PlaySound(L"BossHAttack3", SoundType::SFX, &position);
}

void EnemyBoss::Idle()
{
}

void EnemyBoss::SAttacking()
{
	Vector2 position = GetTransform()->GetLocalPosition();
	SoundManager::Get().PlaySound(L"BossSAttacking", SoundType::SFX, &position);
}

void EnemyBoss::SPreAttacking()
{
	Vector2 position = GetTransform()->GetLocalPosition();
	SoundManager::Get().PlaySound(L"BossSPreAttack", SoundType::SFX, &position);
}

void EnemyBoss::Dead()
{
}