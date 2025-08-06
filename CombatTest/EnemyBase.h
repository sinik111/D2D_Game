#pragma once

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"

#include "EnemyInteract.h"

class PlayerInput;
class Animator;
class RigidBody2D;
class TextRenderer;
class BoxCollider2D;

class EnemyInteract;

class EnemyBase :public Script
{

public:
	virtual ~EnemyBase()
	{}


private:
	//PlayerInput* m_playerInput;

	Animator* m_animator;
	RigidBody2D* m_rigidBody;
	TextRenderer* m_textRenderer;
	BoxCollider2D* m_collider;


	FSM m_fsm;
	FSMContext m_context;

	float m_debugLogTimer = 1.0f;
	float m_dTimer = 0.0f;

	std::unique_ptr<EnemyInteract> m_enemyIA;	


private:
	void Initialize() override;
	void Start() override;
	void FixedUpdate() override;
	void Update() override;
	void OnDestroy() override;

	void OnCollisionEnter(const Collision& collision) override;
	void OnCollisionStay(const Collision& collision) override;
	void OnCollisionExit(const Collision& collision) override;

	void OnTriggerEnter(const Collision& collision) override;
	void OnTriggerExit(const Collision& collision) override;

private:
	void ArrowInput(Vector2 input);
	void vSetting();

public:
	std::wstring animPath[10] = {
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
		HITANDRUN,
		SLOWTURN,		//뒤에서 공격당했을 때, 또는 넉다운에서 회복했을때
	};

	enum TypeOfEnemy
	{
		BASE,
		ZAKO,
		ELITE,
		BOSS,
	};

	//---------------------------------------------------------------------------------
	//int AttackPowerMin;
	//int AttackPowerMax;
	//int	Attack2PowerMin;
	//int Attack2PowerMax;
	//int Attack3PowerMin;
	//int Attack3PowerMax;
	//int AttackKnockdownPower;	

	//---------------------------------------------------------------------------------

	int m_enemyType = BASE;	

	bool m_isDead = false;
	int m_hp = 30;	

	GameObject* m_camera;
	bool m_isInCamera = false;

	int m_nextEnemyState;
	int m_currEnemyState;
	int m_prevEnemyState;

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

	float m_moveSpeed = 200.0f;
	float m_maxSightDistance = 600.0f;
	float m_sightAngle = 90.0f;
	float m_maxRoamDistance = 1000.0f;
	float m_maxChaseDistance = 1200.0f;
	float m_AttackRange = 20.0f;

	bool m_isLockOnTarget = false;
	bool m_isTargetInChaseDist = false;
	bool m_isTargetInRoamDist = false;
	bool m_isTargetInAtkRange = false;
	bool m_isTargetInMaxAtkRange = false;

	float m_atkAngle = 10.0f;
	bool m_isTargetInAtkAngle = false;

	float m_attackInterval = 3.0f;
	float m_attackTimer = 0.0f;

	bool m_isAttackReady = true;
	int m_attackType = 0;

	float m_AttackEffectTime = 0.4f;
	float m_atkColTimer = 0.0f;

	bool m_isUnderAttack = false;
	float m_underAttackValidTime = 0.5f;
	float m_underAttackTimer = 0.0f;

	bool m_isKnockdown = false;
	int m_knockdownResist = 6;
	int m_knockdownAccumulated = 0;
	float m_knockdownPointResetTime = 5.0f;	
	float m_kdPointResetTimer = 0.0f;

	bool m_isKnockback = false;
	int m_knockbackResist = 5;
	Vector2 m_knockbackDirection;
	float m_knockbackSpeed = 0.0f;
	float m_knockbackAcceleration = 0.0f;
	float m_knockbackDuration = 0.5f;
	float m_knockbackCurrentTimer = 0.0f;	

	bool m_isFindPlayer = false;

	float m_evadeDistance = 300.0f;
	float m_evadeProbability = 0.2f;
	float m_attack1Probability = 0.8f;
	float m_attack2Probability = 0.0f;
	float m_attack3Probability = 0.0f;

	bool m_isInCombo = false;

	float m_baseSpeed = 100.0f;
	float m_targetSlowRotationAngle = 0.0f;
	float m_slowRotationSpeed = 100.0f;
	float m_recorveryTime = 1.5f;



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




	void CheckCameraArea();

	void CheckAndTransitState();

	void SetTargetPlayerAndCamera(GameObject* player, GameObject* camera);

	void PositionInit(float x, float y, float angle);

	void MoveTo(const Vector2& destination);

	void Evade();

	void StopMoving();

	void SetAnimDirection();

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

	void CheckKnockdown();

	void SetAttackType(bool& isOnKnockback);

	virtual void EnemyAttackTypeA_A();
	virtual void EnemyAttackTypeA_B();
	virtual void EnemyAttackTypeA_C();

	virtual void EnemyAttackTypeB_A();
	virtual void EnemyAttackTypeB_B();
	virtual void EnemyAttackTypeB_C();

	virtual void EnemyAttackTypeC_A();
	virtual void EnemyAttackTypeC_B();
	virtual void EnemyAttackTypeC_C();





	//void KnockBack(const float& dist, const float& speed);

	

	////////////////////////////


	//void AttackTypeA();
	//void AttackTypeB();
	//void AttackTypeC();




public:

	inline int& Hp() { return m_hp; }

	inline bool& IsDead() { return m_isDead; }

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

	inline float& UnderAttackTimer() {	return m_underAttackTimer;	}

	inline const float& EvadeProbability() { return m_evadeProbability; }

	
	inline bool& IsKnockback() { return m_isKnockback; }	

	inline int& KnockbackResist() { return m_knockbackResist; }

	inline float& KnockbackCurrentTimer() {	return m_knockbackCurrentTimer;	}

	inline Vector2& KnockbackDirection() { return m_knockbackDirection; }

	inline float& KnockbackSpeed() { return m_knockbackSpeed; }


	inline bool& IsKnockdown() { return m_isKnockdown; }

	inline int& KnockdownAccumulated() { return m_knockdownAccumulated; }

	inline int& KnockdownResist() { return m_knockdownResist; }

	inline float& KnockdownPointResetTime() { return m_knockdownPointResetTime; }

	inline float& KdPointResetTimer() { return m_kdPointResetTimer; }


	inline const float& BaseSpeed() { return m_baseSpeed; }

	inline const float& SlowRotationSpeed() { return m_slowRotationSpeed; }

	inline float& TargetSlowRotationAngle() { return m_targetSlowRotationAngle; }

	inline const float& RecorveryTime() { return m_recorveryTime; }

	inline const int& EnemyType() { return m_enemyType; }
	
	

	inline EnemyInteract* EnemyIA() { return m_enemyIA.get(); }


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

