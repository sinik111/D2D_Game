#pragma once

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

#include <array>

#include "EnemyInteract.h"

using AnimPathArray = std::array<std::wstring, 10>;

class PlayerInput;
class Animator;
class RigidBody2D;
class TextRenderer;
class BoxCollider2D;
class CircleCollider;

class EnemyInteract;

class EnemyBase :public Script
{

public:
	EnemyBase();
	virtual ~EnemyBase()
	{
	}

protected:
	Animator* m_animator;
	RigidBody2D* m_rigidBody;
	TextRenderer* m_textRenderer;
	BoxCollider2D* m_collider;
	CircleCollider* m_circleCollider;

	FSM m_fsm;
	FSMContext m_context;

	float m_debugLogTimer = 1.0f;
	float m_dTimer = 0.0f;

	std::unique_ptr<EnemyInteract> m_enemyIA;


	//이쯤부터 막나감
public:
	int m_uniqueId = 0;

	Vector2 m_attackColliderPosition;
	void UpdateAttackColliderPosition();

public:
	std::wstring m_stateText;
	std::wstring m_attackRangeText;
	std::wstring m_distanceText;

protected:
	virtual void Initialize() override;
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void OnDestroy() override;
	virtual void OnRender() override;

	virtual void OnCollisionEnter(const Collision& collision) override;
	virtual void OnCollisionStay(const Collision& collision) override;
	virtual void OnCollisionExit(const Collision& collision) override;

	virtual void OnTriggerEnter(const Collision& collision) override;
	virtual void OnTriggerExit(const Collision& collision) override;

protected:
	virtual void AnimationInit();
	virtual void ColliderInit();
	virtual void PositionInit(Vector2& pos, float angle);

	virtual void StateInit();
	virtual void DebugTextInit();
	virtual void DebugTextUpdate();
	virtual void vSetting();
	virtual void LoadJson(std::wstring path);

public:
	virtual void SetAnimState(int animState);
	virtual void SetAnimDirection();

protected:
	virtual void InitAnimPath(AnimPathArray& animPath, const std::wstring& animName);

	AnimPathArray m_idleAnimPath;
	AnimPathArray m_attack1_1AnimPath;
	AnimPathArray m_attack1_2AnimPath;
	AnimPathArray m_attack2_1AnimPath;
	AnimPathArray m_KnockdownAnimPath;
	AnimPathArray m_moveAnimPath;
	AnimPathArray m_dieAnimPath;

	//AnimPathArray m_hitAnimPath;

	AnimPathArray m_animPath = {
		L"",
		L"",
		L"",
		L""
		L"",
		L"",
		L"",
		L"",
		L"",
		L"",
	};

	AnimPathArray m_animPathdir = {
		L"S",     // 0 
		L"SW",    // 1
		L"S",     // 2 
		L"SE",    // 3
		L"W",     // 4
		L"S",     // 5 
		L"E",     // 6
		L"NW",    // 7
		L"N",     // 8
		L"NE"     // 9
	};


public:

	enum Dir	//Num키 기준으로 배열. 0은 랜덤(나중에 쓰게 된다면..), 5는 NONE(값 없음)
	{
		RANDOM,			//0
		LEFT_DOWN,		//1
		DOWN,			//2
		RIGHT_DOWN,		//3
		LEFT,			//4
		NONE,			//5
		RIGHT,			//6
		LEFT_UP,		//7	
		UP,				//8
		RIGHT_UP,		//9	
	};


	enum State
	{
		NOSTATE,
		OUTOFAREA,		//카메라 범위 밖	
		INAREA,			//카메라 범위 안
		ENGAGE,			//이동 가능한 위치에 있는 플레이어 발견
		ONATTACK,
		ONATTACK2,
		ONATTACK3,
		RETURN,
		ONEVADE,
		KNOCKDOWN,
		GRABBED,
		HITANDRUN,
		EVADEANDRUN,
		SLOWTURN,		//뒤에서 공격당했을 때, 또는 넉다운에서 회복했을때		
		DEAD,
	};

	enum AnimState
	{
		NoAnimation,
		EnemyAnimIdle,
		EnemyAnimAttack1_1,
		EnemyAnimAttack1_2,
		EnemyAnimAttack2_1,
		EnemyAnimKnockdown,
		EnemyAnimMove,
		EnemyAnimDie,
		EnemyAnimMoveIn,
		EnemyAnimMoveOut,
	};

	enum TypeOfEnemy
	{
		BASE,
		ZAKO,
		ELITE,
		BOSS,
	};


	//---------------------------------------------------------------------------------


	int m_enemyType = BASE;
	GameObject* m_camera;
	bool m_isInCamera = false;
	bool m_isDead = false;
	int m_nextEnemyState;
	int m_currEnemyState;
	int m_prevEnemyState;

	int m_nextAnimationState;
	int m_currAnimationState;
	int m_prevAnimationState;

	float m_rotationAngle = 0.0f;
	int m_direction = DOWN;
	int m_prevDir;
	int m_currDir;

	GameObject* m_player;
	bool m_isPlayerDead = false;

	Vector2 m_originPos;
	float m_originAngle;
	bool m_toDoMove = false;
	float m_distanceToMoveSq = 0.0f;
	Vector2 m_movingDestPos;
	Vector2 m_departurePos;
	Vector2 m_aheadDirection;

	float m_acceleration = 0.0f;

	bool m_isLockOnTarget = false;
	bool m_isTargetInChaseDist = false;
	bool m_isTargetInRoamDist = false;
	bool m_isTargetInAtkRange = false;
	bool m_isTargetInMaxAtkRange = false;
	bool m_isTargetInAtkAngle = false;

	float m_attackTimer = 0.0f;
	bool m_isAttackReady = true;
	int m_attackType = 0;
	bool m_isInCombo = false;

	bool m_isUnderAttack = false;
	float m_underAttackValidTime = 0.03f;
	float m_underAttackTimer = 0.0f;

	bool m_isKnockdown = false;
	int m_knockdownAccumulated = 0;
	float m_kdPointResetTimer = 0.0f;

	bool m_isKnockback = false;
	Vector2 m_knockbackDirection;
	float m_knockbackSpeed = 0.0f;
	float m_knockbackAcceleration = 0.0f;
	float m_knockbackDuration = 0.5f;
	float m_knockbackCurrentTimer = 0.0f;

	bool m_isGrabbed = false;
	float m_grabTimer = 3.0f;

	bool m_isFindPlayer = false;

	float m_targetSlowRotationAngle = 000.0f;
	float m_slowRotationSpeed = 200.0f;
	bool m_doDeleteThisObj = false;
	float m_attackWaitTimer = 0.0f;
	bool m_isAttackWait = false;

	GameObject* m_enemyAttack;

	float m_attackDuration_1A = 1.0f;
	float m_attackDuration_1B = 1.0f;
	float m_attackDuration_1C = 1.0f;
	float m_attackDuration_2A = 1.0f;
	float m_attackDuration_2B = 1.0f;
	float m_attackDuration_2C = 1.0f;
	float m_attackDuration_3A = 1.0f;
	float m_attackDuration_3B = 1.0f;
	float m_attackDuration_3C = 1.0f;

	float m_attackRadius_1A = 130.0f;
	float m_attackRadius_1B = 130.0f;
	float m_attackRadius_1C = 130.0f;
	float m_attackRadius_2A = 130.0f;
	float m_attackRadius_2B = 130.0f;
	float m_attackRadius_2C = 130.0f;
	float m_attackRadius_3A = 130.0f;
	float m_attackRadius_3B = 130.0f;
	float m_attackRadius_3C = 130.0f;

	float m_attackAngle_1A = 30.0f;
	float m_attackAngle_1B = 30.0f;
	float m_attackAngle_1C = 30.0f;
	float m_attackAngle_2A = 30.0f;
	float m_attackAngle_2B = 30.0f;
	float m_attackAngle_2C = 30.0f;
	float m_attackAngle_3A = 30.0f;
	float m_attackAngle_3B = 30.0f;
	float m_attackAngle_3C = 30.0f;

	//float m_recorveryTime = 1.5f;

	int m_maxHp = 30;
	int m_hp = 30;
	float m_moveSpeed = 200.0f;
	float m_moveSpeedOrigin = 200.0f;
	float m_moveSpeedEvade = 300.0f;
	float m_maxSightDistance = 600.0f;
	float m_sightAngle = 90.0f;
	float m_maxRoamDistance = 250.0f/*1000.0f*/;
	float m_maxChaseDistance = 1200.0f;
	float m_AttackRange = 100.0f;
	float m_atkAngle = 10.0f;
	float m_attackInterval = 3.0f;
	int m_knockdownResist = 6;
	float m_knockdownPointResetTime = 5.0f;
	int m_knockbackResist = 5;

	float m_evadeDistance = 300.0f;
	float m_evadeProbability = 20.0f;
	float m_attack1Probability = 80.0f;
	float m_attack2Probability = 0.0f;
	float m_attack3Probability = 0.0f;

	int m_AttackPowerMin = 3;
	int m_AttackPowerMax = 4;
	int	m_Attack2PowerMin = 0;
	int m_Attack2PowerMax = 0;
	int m_Attack3PowerMin = 0;
	int m_Attack3PowerMax = 0;
	int m_AttackKnockdownPower;

public:
	float m_attackA_BProb = 0.6f;
	float m_attackA_CProb = 0.4f;

	float m_attackB_BProb = 0.5f;
	float m_attackB_CProb = 0.5f;

	float m_attackC_BProb = 0.5f;
	float m_attackC_CProb = 0.5f;

	float m_attackA_ATime = 1.0f;
	float m_attackA_BTime = 1.0f;
	float m_attackA_CTime = 1.0f;

	float m_attackB_ATime = 1.0f;
	float m_attackB_BTime = 1.0f;
	float m_attackB_CTime = 1.0f;

	float m_attackC_ATime = 1.0f;
	float m_attackC_BTime = 1.0f;
	float m_attackC_CTime = 1.0f;

	float m_attack1AMoveSpd = 300.0f;
	float m_attack1BMoveSpd = 300.0f;
	float m_attack1CMoveSpd = 300.0f;

	float m_attack2AMoveSpd = 300.0f;
	float m_attack2BMoveSpd = 300.0f;
	float m_attack2CMoveSpd = 300.0f;

	float m_attack3AMoveSpd = 300.0f;
	float m_attack3BMoveSpd = 300.0f;
	float m_attack3CMoveSpd = 300.0f;

	float m_attack1AMoveDist = 30.0f;
	float m_attack1BMoveDist = 30.0f;
	float m_attack1CMoveDist = 30.0f;

	float m_attack2AMoveDist = 30.0f;
	float m_attack2BMoveDist = 30.0f;
	float m_attack2CMoveDist = 30.0f;

	float m_attack3AMoveDist = 30.0f;
	float m_attack3BMoveDist = 30.0f;
	float m_attack3CMoveDist = 30.0f;

	bool m_isMovingOnTime = false;
	float m_moveOnTimeDuration = 0.0f;
	float m_moveOnTimeTimer = 0.0f;

	int m_evadeType = 1;
	bool m_isOnEvade = false;
	float m_oddEvadeDirection = 1.0f;

	float m_bossMoveInDuration = 0.4f;
	float m_bossMoveInTimer = 0.0f;
	float m_bossMoveOutDuration = 0.417f;
	float m_bossMoveOutTimer = 0.0f;

	bool m_bossMoveCheck = false;

	float m_attack1ASafetyTime = 0.5f;
	float m_attack1BSafetyTime = 0.5f;
	float m_attack1CSafetyTime = 0.5f;
	float m_attack2ASafetyTime = 0.5f;
	float m_attack2BSafetyTime = 0.5f;
	float m_attack2CSafetyTime = 0.5f;
	float m_attack3ASafetyTime = 0.5f;
	float m_attack3BSafetyTime = 0.5f;
	float m_attack3CSafetyTime = 0.5f;

	bool m_isGetParried = false;

	float m_hitOverayDuration = 0.1f;
	float m_hitOverayTimer = 0.0f;
	float m_isHitOverayDraw = false;

	void HitOverayManager();

	void HitStopManager()
	{

	}


	inline void SetIsGetParried()
	{
		if (m_isDead == false)
		{
			if (!m_isUnderAttack)
				EnemyUnderAttack();

			m_isGetParried = true;
		}
	}

	bool m_isGetParriedHeavy = false;

	inline void SetIsGetParriedHeavy()
	{
		if (m_isDead == false)
		{
			if (!m_isUnderAttack)
				EnemyUnderAttack();
			m_isGetParriedHeavy = true;
		}
	}

	bool m_isGetDealed = false;

	inline void SetIsGetDealed()
	{
		if (m_isDead == false)
		{
			if (!m_isUnderAttack)
				EnemyUnderAttack();

			m_isGetDealed = true;
		}
	}

	bool m_isGetDealedHeavy = false;

	inline void SetIsGetDealedHeavy()
	{
		if (m_isDead == false)
		{
			if (!m_isUnderAttack)
				EnemyUnderAttack();

			m_isGetDealedHeavy = true;
		}
	}

	void CheckEnemyHit();


	void UpdateBossAnim();

	void CheckCameraArea();

	virtual void CheckAndTransitState();

	virtual void CheckAndTransitAnimState();

	void SetTargetPlayerAndCamera(GameObject* player, GameObject* camera);

	void MoveTo(const Vector2& destination);

	void MoveOnTime(float deltaTime);

	void Evade(int evadeType);

	void StopMoving();

	void UpdateDirection();

	void CheckStopMoving();

	void RigidBodyUpdate();

	void SetRotationAngle(float angle);

	void SetAngleByDirection(int n);

	void SetAngleByAheadDirection(const Vector2& adir);

	void AttackCoolCheck();

	void UnderAttackManage();

	void AheadToTarget();

	void UpdateKnockback();

	void StopKnockback();

	void EnemyUnderAttack(const Collision& collision);

	void CheckKnockdownPointReset();

	void CheckGrabbed();

	void CheckDead();

	void AssaultMove();

	void SetAttackType(bool& isOnKnockback, bool& isTargetInRoamDist);

	virtual void EnemyAttackTypeA_A();
	virtual void EnemyAttackTypeA_B();
	virtual void EnemyAttackTypeA_C();

	virtual void EnemyAttackTypeB_A();
	virtual void EnemyAttackTypeB_B();
	virtual void EnemyAttackTypeB_C();

	virtual void EnemyAttackTypeC_A();
	virtual void EnemyAttackTypeC_B();
	virtual void EnemyAttackTypeC_C();

	void EnemyUnderAttack();

	void KnockBack(float dist, float speed);

public:

	inline int& Hp() { return m_hp; }

	inline bool& IsDead() { return m_isDead; }

	inline bool& DoDeleteThisObj() { return m_doDeleteThisObj; }

	inline int& CurrEnemyState() { return m_currEnemyState; }

	inline int& PrevEnemyState() { return m_prevEnemyState; }

	inline int& PrevDir() { return m_prevDir; }
	inline int& CurrDir() { return m_currDir; }

	inline Vector2& OriginPos() { return m_originPos; }
	inline float& OriginAngle() { return m_originAngle; }

	inline Vector2& DepaturePos() { return m_departurePos; }

	inline float& DistanceToMoveSq() { return m_distanceToMoveSq; }

	inline Vector2& MovingDestPos() { return m_movingDestPos; }

	inline int& Direction() { return m_direction; }					//Num패드 숫자 기준 방향 

	inline Vector2& AheadDirection() { return m_aheadDirection; }	//Vector2 타입 방향

	inline float& RotationAngle() { return m_rotationAngle; }		//Degree 타입 방향

	inline float& MoveSpeed() { return m_moveSpeed; }

	inline float& Acceleration() { return m_acceleration; }

	inline bool& ToDoMove() { return m_toDoMove; }


	inline float& MaxSightDistance() { return m_maxSightDistance; }

	inline float& SightAngle() { return m_sightAngle; }


	inline float& MaxRoamDistance() { return m_maxRoamDistance; }

	inline float& MaxChaseDistance() { return m_maxChaseDistance; }

	inline float& AttackRange() { return m_AttackRange; }


	inline const Vector2& CameraPos() { return m_camera->GetTransform()->GetLocalPosition(); }

	inline bool& IsInCamera() { return m_isInCamera; }


	inline const Vector2& TargetPos() { return m_player->GetTransform()->GetLocalPosition(); }

	inline bool& IsLockOnTarget() { return m_isLockOnTarget; }

	inline bool& IsTargetInChaseDist() { return m_isTargetInRoamDist; }

	inline bool& IsTargetInRoamDist() { return m_isTargetInChaseDist; }

	inline bool& IsTargetInAtkRange() { return m_isTargetInAtkRange; }

	inline bool& IsTargetInMaxAtkRange() { return m_isTargetInMaxAtkRange; }

	inline float& AtkAngle() { return m_atkAngle; }

	inline float& AttackTimer() { return m_attackTimer; }

	inline float& AttackInterval() { return m_attackInterval; }

	inline bool& IsTargetInAtkAngle() { return m_isTargetInAtkAngle; }

	inline bool& IsAttackReady() { return m_isAttackReady; }

	inline bool& IsInCombo() { return m_isInCombo; }


	inline const int& AttackType() { return m_attackType; }

	inline bool& IsFindPlayer() { return m_isFindPlayer; }


	inline bool& IsUnderAttack() { return m_isUnderAttack; }

	inline float& UnderAttackTimer() { return m_underAttackTimer; }

	inline const float& EvadeProbability() { return m_evadeProbability; }


	inline bool& IsKnockback() { return m_isKnockback; }

	inline int& KnockbackResist() { return m_knockbackResist; }

	inline float& KnockbackCurrentTimer() { return m_knockbackCurrentTimer; }

	inline Vector2& KnockbackDirection() { return m_knockbackDirection; }

	inline float& KnockbackSpeed() { return m_knockbackSpeed; }


	inline bool& IsKnockdown() { return m_isKnockdown; }

	inline int& KnockdownAccumulated() { return m_knockdownAccumulated; }

	inline int& KnockdownResist() { return m_knockdownResist; }

	inline float& KnockdownPointResetTime() { return m_knockdownPointResetTime; }

	inline float& KdPointResetTimer() { return m_kdPointResetTimer; }


	inline bool& IsGrabbed() { return m_isGrabbed; }

	inline float& GrabTimer() { return m_grabTimer; }


	inline const float& SlowRotationSpeed() { return m_slowRotationSpeed; }

	inline float& TargetSlowRotationAngle() { return m_targetSlowRotationAngle; }

	//inline const float& RecorveryTime() { return m_recorveryTime; }

	inline const int& EnemyType() { return m_enemyType; }



	inline EnemyInteract* EnemyIA()
	{
		if (!m_enemyIA)
			return nullptr;

		return m_enemyIA.get();
	}

	inline const bool& IsPlayerDead()
	{
		m_context.boolParams[L"IsPlayerDead"] = m_isPlayerDead;
		return m_context.boolParams[L"IsPlayerDead"];
	}

	inline const bool IsPlayerNull()
	{
		if (m_player == nullptr)
			return true;

		return false;
	}

	RigidBody2D* RigidBody();

	const Vector2& Pos();


};

